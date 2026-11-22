#include "device_info.h"

#include <vulkan/vulkan.h>

#include <dirent.h>

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {

std::string json_escape(const std::string& s) {
    std::string out;
    for (unsigned char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (c < 0x20) {
                    char buf[8];
                    std::snprintf(buf, sizeof(buf), "\\u%04x", c);
                    out += buf;
                } else {
                    out += static_cast<char>(c);
                }
        }
    }
    return out;
}

std::string json_string(const std::string& s) {
    return "\"" + json_escape(s) + "\"";
}

std::string read_text(const std::string& path) {
    std::ifstream f(path);
    if (!f) return std::string();
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

std::string trim(const std::string& s) {
    const auto b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return std::string();
    const auto e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

const char* device_type_name(VkPhysicalDeviceType t) {
    switch (t) {
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "integrated";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "discrete";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "virtual";
        case VK_PHYSICAL_DEVICE_TYPE_CPU: return "cpu";
        default: return "other";
    }
}

struct GpuInfo {
    bool ok = false;
    std::string name;
    std::string driver_version;
    std::string api_version;
    std::string vendor_id;
    std::string device_type;
    std::uint64_t device_local_bytes = 0;
    std::uint32_t max_image_2d = 0;
    std::uint32_t device_count = 0;
};

GpuInfo query_vulkan() {
    GpuInfo info;

    VkApplicationInfo app{};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pApplicationName = "vr-companion";
    app.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create{};
    create.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create.pApplicationInfo = &app;

    VkInstance instance = VK_NULL_HANDLE;
    if (vkCreateInstance(&create, nullptr, &instance) != VK_SUCCESS) return info;

    uint32_t count = 0;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    if (count > 0) {
        std::vector<VkPhysicalDevice> devices(count);
        vkEnumeratePhysicalDevices(instance, &count, devices.data());
        info.device_count = count;

        // The first device is the one Android presents as the primary GPU.
        const VkPhysicalDevice dev = devices[0];
        VkPhysicalDeviceProperties props{};
        vkGetPhysicalDeviceProperties(dev, &props);
        VkPhysicalDeviceMemoryProperties mem{};
        vkGetPhysicalDeviceMemoryProperties(dev, &mem);

        for (uint32_t i = 0; i < mem.memoryHeapCount; ++i) {
            if (mem.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                info.device_local_bytes += mem.memoryHeaps[i].size;
            }
        }

        char buf[64];
        info.name = props.deviceName;
        std::snprintf(buf, sizeof(buf), "%u.%u.%u", VK_API_VERSION_MAJOR(props.apiVersion),
                      VK_API_VERSION_MINOR(props.apiVersion), VK_API_VERSION_PATCH(props.apiVersion));
        info.api_version = buf;
        std::snprintf(buf, sizeof(buf), "0x%08x", props.driverVersion);
        info.driver_version = buf;
        std::snprintf(buf, sizeof(buf), "0x%04x", props.vendorID);
        info.vendor_id = buf;
        info.device_type = device_type_name(props.deviceType);
        info.max_image_2d = props.limits.maxImageDimension2D;
        info.ok = true;
    }

    vkDestroyInstance(instance, nullptr);
    return info;
}

std::string cpu_json() {
    std::uint32_t cores = 0;
    const std::string cpuinfo = read_text("/proc/cpuinfo");
    std::istringstream lines(cpuinfo);
    std::string model;
    for (std::string line; std::getline(lines, line);) {
        if (line.rfind("processor", 0) == 0) ++cores;
        if (model.empty() && (line.rfind("Hardware", 0) == 0 || line.rfind("model name", 0) == 0)) {
            const auto colon = line.find(':');
            if (colon != std::string::npos) model = trim(line.substr(colon + 1));
        }
    }
    std::ostringstream out;
    out << "{\"logical_cores\":" << cores << ",\"model\":" << (model.empty() ? std::string("null") : json_string(model)) << "}";
    return out.str();
}

std::string memory_json() {
    std::istringstream lines(read_text("/proc/meminfo"));
    for (std::string line; std::getline(lines, line);) {
        if (line.rfind("MemTotal:", 0) == 0) {
            std::istringstream fields(line.substr(9));
            std::uint64_t kb = 0;
            fields >> kb;
            return "{\"total_kb\":" + std::to_string(kb) + "}";
        }
    }
    return "null";
}

std::string thermal_json() {
    std::vector<std::string> zones;
    if (DIR* dir = opendir("/sys/class/thermal")) {
        while (dirent* entry = readdir(dir)) {
            const std::string name = entry->d_name;
            if (name.rfind("thermal_zone", 0) == 0) zones.push_back(name);
        }
        closedir(dir);
    }
    std::string out = "[";
    bool first = true;
    for (const auto& z : zones) {
        const std::string base = "/sys/class/thermal/" + z + "/";
        const std::string type = trim(read_text(base + "type"));
        const std::string temp = trim(read_text(base + "temp"));
        if (!first) out += ",";
        first = false;
        out += "{\"zone\":" + json_string(z) + ",\"type\":" + json_string(type.empty() ? std::string("unavailable") : type) +
               ",\"temp_mC\":" + (temp.empty() ? std::string("null") : temp) + "}";
    }
    return out + "]";
}

}  // namespace

std::string collect_device_info_json() {
    const GpuInfo gpu = query_vulkan();
    std::ostringstream out;
    out << "{\"gpu\":";
    if (gpu.ok) {
        out << "{\"name\":" << json_string(gpu.name)
            << ",\"type\":" << json_string(gpu.device_type)
            << ",\"vulkan_api\":" << json_string(gpu.api_version)
            << ",\"driver_version\":" << json_string(gpu.driver_version)
            << ",\"vendor_id\":" << json_string(gpu.vendor_id)
            << ",\"device_local_bytes\":" << gpu.device_local_bytes
            << ",\"max_image_2d\":" << gpu.max_image_2d
            << ",\"device_count\":" << gpu.device_count << "}";
    } else {
        out << "null";
    }
    out << ",\"cpu\":" << cpu_json() << ",\"memory\":" << memory_json() << ",\"thermal\":" << thermal_json() << "}";
    return out.str();
}
