cmake_minimum_required(VERSION 3.22)
project(vrcompanion_native CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform only: Vulkan loader and Android log come from the NDK. No third-party code.
add_library(vrcompanion SHARED device_info.cpp jni_bridge.cpp)
target_link_libraries(vrcompanion android log vulkan)
target_compile_options(vrcompanion PRIVATE -Wall -Wextra)
