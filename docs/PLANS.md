# Companion plans

- Target: Meta Quest and Android XR headsets only (owner). Not a phone app.
- Streaming transport: ALVR (MIT), approved (owner). Pinned to v20.14.1, commit a9f6542. Integration pending.
- Native device info (done): Vulkan GPU, CPU, memory, and thermal data through the NDK.
- Builds: debug and signed release APKs, using the shared Android signing identity (secrets are stored in this repo's Actions secrets; their values are not in the repo).
- Next: wire the ALVR headset client into the Gradle and CMake build, keep upstream unmodified, and keep any change as a separate patch.
- Later: a GLES fallback for device info; sharing device info to the render host.
