#include <jni.h>

#include "device_info.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_enderman_vrcompanion_DeviceInfo_nativeGetJson(JNIEnv* env, jclass) {
    return env->NewStringUTF(collect_device_info_json().c_str());
}
