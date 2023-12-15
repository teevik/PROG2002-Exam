cmake_minimum_required(VERSION 3.15)
project(framework)

add_library(framework
        include/framework/Shader.h
        src/Shader.cpp

        include/framework/window.h
        src/window.cpp

        include/framework/geometry.h
        src/geometry.cpp
        
        include/framework/Texture.h
        src/Texture.cpp

        include/framework/Camera.h
        src/Camera.cpp
        
        include/framework/IndexBuffer.h
        src/IndexBuffer.cpp

        include/framework/VertexArray.h
        include/framework/UniformBuffer.h
        include/framework/VertexBuffer.h)

target_include_directories(framework PUBLIC include)

target_link_libraries(framework PUBLIC glad glfw glm stb)

target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)