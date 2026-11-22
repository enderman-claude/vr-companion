# Companion app working notes

Living document. Update at the end of every work session.

## Status
- Milestone 0: repo, MIT license, CMake skeleton, CI on Linux and Windows.

## Milestones
- **M0 Foundation** — repo, CMake, CI, stub executable. *Current.*
- **M1 Transport** — connection to the engine's render client (LAN first). Evaluate Moonlight/Sunshine-style protocols, but only if the license is permissive enough.
- **M2 Decode and present** — hardware video decode, present frames to the headset.
- **M3 Input and latency** — headset pose return path, latency measurement.
- **M4 Linux host support** — first-class Linux path (Steam Link-style use case).
- **M5 Android client** — stubs first, real build later.

## Decisions
- MIT license. Permissive dependencies only.
- Same CI pattern as vr-engine (Linux + Windows builds).

## Session log
- Session 1: repo, license, CMake skeleton, CI, stub main.

## Build and signing
- The APK build copies camera-app's pipeline: JDK 17, Gradle 8.7, Kotlin DSL, AGP 8.5.2, the keystore decoded from CAMERA_KEYSTORE_B64 into the runner's temp folder, debug and release APKs, error annotations, and an APK listing.
- Signing secrets (CAMERA_KEYSTORE_B64, CAMERA_KEYSTORE_PASSWORD, CAMERA_KEY_ALIAS, CAMERA_KEY_PASSWORD) are the shared identity for new Android apps, copied from enderman-claude/camera-app-signing.
- Lesson: set secrets with `printf '%s' "$VALUE" | gh secret set NAME`. A trailing newline in a password breaks the keystore read.
- Quest: manifest includes the com.oculus.intent.category.VR category. Android XR needs its own check later.

## Device info (native)
- The app shows GPU, CPU, memory, and thermal data. Source: android/app/src/main/cpp/device_info.cpp.
- GPU: Vulkan through the NDK (vulkan loader, arm64-v8a, minSdk 29). Reports device name, type, Vulkan API version, driver version, vendor ID, device-local memory, and max 2D image size.
- CPU: /proc/cpuinfo (logical cores, model). Memory: /proc/meminfo (MemTotal). Thermal: /sys/class/thermal (readable zones only).
- Third-party code: none. Vulkan and the Android log come from the NDK, which is platform SDK, not an open source dependency. The open-source-reuse skill's approval step applies to any future third-party library (for example, a JSON library).
- Not verified: the JSON values on a real Quest or phone. CI only proves the native module compiles and links.
