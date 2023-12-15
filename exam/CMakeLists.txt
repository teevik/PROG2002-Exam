cmake_minimum_required(VERSION 3.15)
project(exam)

# Locate the OpenGL package on the system
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        data_structures/SparseGrid.h
        Floor.cpp
        Floor.h
        Player.cpp
        Player.h
        Object.cpp
        Object.h)

# Libraries to link with
target_link_libraries(${PROJECT_NAME} glm glfw glad OpenGL::GL stb framework)

# Enable swizzle in glm, for example `vec4::xyz()`
target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_SWIZZLE)

# add_custom_command(
#         TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_CURRENT_SOURCE_DIR}/resources
#         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)

# target_compile_definitions(${PROJECT_NAME} PRIVATE
#         RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/")