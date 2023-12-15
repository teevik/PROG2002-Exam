#ifndef EXAMAUTUMN2023_OBJECT_H
#define EXAMAUTUMN2023_OBJECT_H

#include <memory>
#include "framework/Shader.h"
#include "framework/VertexArray.h"

class Object {
private:
    static std::shared_ptr<framework::Shader> shader;

public:
    struct Vertex {
        /// Vertex position
        glm::vec3 position;

        /// Vertex color
        glm::vec4 color;
    };

    inline static const std::vector<framework::VertexAttribute> VERTEX_ATTRIBUTES = {
        {.type = GL_FLOAT, .size = 3, .offset = offsetof(Vertex, position)},
        {.type = GL_FLOAT, .size = 4, .offset = offsetof(Vertex, color)},
    };

    static std::shared_ptr<framework::Shader> getShader();
};

#endif //EXAMAUTUMN2023_OBJECT_H
