#ifndef EXAMAUTUMN2023_FLOOR_H
#define EXAMAUTUMN2023_FLOOR_H

#include "glm/fwd.hpp"
#include "glm/detail/type_vec2.hpp"
#include "glm/detail/type_vec3.hpp"
#include "framework/VertexArray.h"
#include "framework/Camera.h"

struct Floor {
    struct Vertex {
        /// Vertex position
        glm::vec2 position;

        /// The corresponding grid position for this vertex
        glm::uvec2 gridPosition;
    };

    inline static const std::vector<framework::VertexAttribute> VERTEX_ATTRIBUTES = {
        {.type = GL_FLOAT, .size = 2, .offset = offsetof(Vertex, position)},
        {.type = GL_UNSIGNED_INT, .size = 2, .offset = offsetof(Vertex, gridPosition)},
    };

    const glm::uvec2 size;
    const std::shared_ptr<framework::Shader> shader;
    const framework::VertexArray<Vertex> vertexArray;

    static Floor create(glm::uvec2 size);

    void draw(const framework::Camera &) const;
};

#endif //EXAMAUTUMN2023_FLOOR_H
