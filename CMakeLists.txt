cmake_minimum_required(VERSION 3.15)
project(ExamAutumn2023)

# C++ 20
set(CMAKE_CXX_STANDARD 20)

# Disable compiler-specific extensions to the C++ standard
set(CMAKE_CXX_EXTENSIONS OFF)

# Locate the OpenGL package on the system
find_package(OpenGL REQUIRED)

# Define the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# GLFW cmake settings
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)

# Add external dependencies
add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

# Header-only interface for the 'stb' library
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE external/stb)

# Framework
add_subdirectory(framework)

# Exam
add_subdirectory(exam)