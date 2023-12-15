#ifndef EXAMAUTUMN2023_PLAYER_H
#define EXAMAUTUMN2023_PLAYER_H

#include "framework/VertexArray.h"
#include "Object.h"
#include "framework/Camera.h"

struct Player {
    const std::shared_ptr<framework::Shader> shader;
    const framework::VertexArray<Object::Vertex> vertexArray;

    static Player create();

    void draw(
        glm::uvec2 position,
        const framework::Camera &
    ) const;
};

#endif //EXAMAUTUMN2023_PLAYER_H
