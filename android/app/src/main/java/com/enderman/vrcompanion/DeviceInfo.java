package com.enderman.vrcompanion;

// Reads GPU, CPU, memory, and thermal data from the native layer (Vulkan and system files).
public final class DeviceInfo {
    static {
        System.loadLibrary("vrcompanion");
    }

    private DeviceInfo() {}

    // Returns a JSON document; see device_info.cpp for the fields.
    public static native String nativeGetJson();
}
