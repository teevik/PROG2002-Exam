#ifndef EXAMAUTUMN2023_OBJECT_H
#define EXAMAUTUMN2023_OBJECT_H

#include "framework/VertexArray.h"
#include "Object.h"
#include "framework/Camera.h"

struct Object {
    struct Vertex {
        /// Vertex position
        glm::vec3 position;

        /// Vertex color
        glm::vec3 color;
    };

    inline static const std::vector<framework::VertexAttribute> VERTEX_ATTRIBUTES = {
        {.type = GL_FLOAT, .size = 3, .offset = offsetof(Vertex, position)},
        {.type = GL_FLOAT, .size = 3, .offset = offsetof(Vertex, color)},
    };

    // Shared shader
    static std::shared_ptr<framework::Shader> shader;

    explicit Object(framework::VertexArray<Object::Vertex> vertexArray);

//    // Move constructor
    Object(Object &&object) noexcept;

    framework::VertexArray<Object::Vertex> vertexArray;

    static Object create(glm::vec3 color);

    void draw(
        glm::uvec2 position,
        const framework::Camera &
    ) const;
};

#endif //EXAMAUTUMN2023_OBJECT_H
