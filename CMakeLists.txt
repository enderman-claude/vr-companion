cmake_minimum_required(VERSION 3.20)
project(vr_companion VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(vr_companion src/main.cpp)

if(MSVC)
    target_compile_options(vr_companion PRIVATE /W4 /WX)
else()
    target_compile_options(vr_companion PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
