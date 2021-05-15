cmake_minimum_required(VERSION 3.10)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(glfw)

add_subdirectory(glm)
add_subdirectory(tomlplusplus)

# STB does not use CMake, so we create a minimal header-only target
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb/)

# Glad does not use CMake, so we create a minimal target
add_library(glad glad/src/glad.c)
if(NOT WIN32)
    target_link_libraries(glad PUBLIC dl)
endif()
target_include_directories(glad PUBLIC glad/include/)