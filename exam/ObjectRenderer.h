#ifndef EXAMAUTUMN2023_OBJECTRENDERER_H
#define EXAMAUTUMN2023_OBJECTRENDERER_H

#include "framework/VertexArray.h"
#include "ObjectRenderer.h"
#include "framework/Camera.h"

/// Renders objects that has a simple vertex specification
struct ObjectRenderer {
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

    framework::VertexArray<ObjectRenderer::Vertex> vertexArray;

    static ObjectRenderer create(glm::vec3 color);

    void draw(glm::uvec2 position, const framework::Camera &camera) const;
};

#endif //EXAMAUTUMN2023_OBJECTRENDERER_H
