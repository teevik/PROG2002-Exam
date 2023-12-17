#ifndef EXAMAUTUMN2023_OBJECTRENDERER_H
#define EXAMAUTUMN2023_OBJECTRENDERER_H

#include "framework/VertexArray.h"
#include "framework/Camera.h"
#include "framework/Texture.h"
#include "Light.h"

/// Renders objects that has a simple vertex specification
struct ObjectRenderer {
    struct Vertex {
        /// Vertex position
        glm::vec3 position;

        /// Cubemap texture coordinates
        glm::vec3 textureCoordinates;

        /// Normals
        glm::vec3 normal;
    };

    inline static const std::vector<framework::VertexAttribute> VERTEX_ATTRIBUTES = {
        {.type = GL_FLOAT, .size = 3, .offset = offsetof(Vertex, position)},
        {.type = GL_FLOAT, .size = 3, .offset = offsetof(Vertex, textureCoordinates)},
        {.type = GL_FLOAT, .size = 3, .offset = offsetof(Vertex, normal)},
    };

    using Mesh = std::vector<Vertex>;

    // Shared shader
    static std::shared_ptr<framework::Shader> shader;

    framework::VertexArray<ObjectRenderer::Vertex> vertexArray;

    static ObjectRenderer create();

    void draw(
        glm::vec2 position,
        glm::vec3 color,
        const framework::Texture *texture,
        const Light &light,
        const framework::Camera &camera,
        bool useTextures,
        glm::vec3 glow_color = glm::vec3(0.f)
    ) const;
};

#endif //EXAMAUTUMN2023_OBJECTRENDERER_H
