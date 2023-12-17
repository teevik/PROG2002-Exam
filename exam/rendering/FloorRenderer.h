#ifndef EXAMAUTUMN2023_FLOORRENDERER_H
#define EXAMAUTUMN2023_FLOORRENDERER_H

#include "glm/fwd.hpp"
#include "glm/detail/type_vec2.hpp"
#include "glm/detail/type_vec3.hpp"
#include "framework/VertexArray.h"
#include "framework/Camera.h"
#include "framework/Texture.h"
#include "Light.h"

/// Renders the floor with storage locations as circles
struct FloorRenderer {
    struct Vertex {
        /// Vertex position
        glm::vec2 position;
    };

    inline static const std::vector<framework::VertexAttribute> VERTEX_ATTRIBUTES = {
        {.type = GL_FLOAT, .size = 2, .offset = offsetof(Vertex, position)},
    };

    // Needs to comply with std140, so each data needs to be 16 bytes long
    struct StorageLocation {
        glm::uvec2 position; // 8 bytes
        glm::vec2 _padding; // 8 bytes of padding
    };

    const std::shared_ptr<framework::Shader> shader;
    const framework::VertexArray<Vertex> vertexArray;
    const framework::UniformBuffer<StorageLocation> storageLocationsBuffer;
    const framework::Texture texture;

    static FloorRenderer create(glm::uvec2 size, const std::vector<glm::uvec2> &storageLocations);

    void draw(const Light &light, const framework::Camera &camera, bool useTextures) const;
};

#endif //EXAMAUTUMN2023_FLOORRENDERER_H
