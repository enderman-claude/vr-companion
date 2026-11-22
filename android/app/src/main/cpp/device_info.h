#pragma once
#include <string>

// Returns a JSON document describing this device's GPU (via Vulkan), CPU, memory,
// and thermal zones. Fields that cannot be read are reported as null or "unavailable".
std::string collect_device_info_json();
