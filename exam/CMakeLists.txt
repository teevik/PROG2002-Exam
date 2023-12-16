cmake_minimum_required(VERSION 3.15)
project(exam)

# Locate the OpenGL package on the system
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        rendering/FloorRenderer.cpp
        rendering/FloorRenderer.h
        rendering/ObjectRenderer.cpp
        rendering/ObjectRenderer.h
        constants.h
        models.h
        GameState.cpp
        GameState.h
        game/Board.cpp
        game/Board.h
        rendering/BoardRenderer.cpp
        rendering/BoardRenderer.h)

# Libraries to link with
target_link_libraries(${PROJECT_NAME} glm glfw glad OpenGL::GL stb framework)

# Enable swizzle in glm, for example `vec4::xyz()`
target_compile_definitions(${PROJECT_NAME} PRIVATE GLM_FORCE_SWIZZLE)

# Include resource folder with build
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)

#  path to resource folder @formatter:off
target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/")