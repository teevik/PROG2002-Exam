#ifndef EXAMAUTUMN2023_MODELS_H
#define EXAMAUTUMN2023_MODELS_H

#include <vector>
#include "framework/geometry.h"
#include "rendering/ObjectRenderer.h"

static ObjectRenderer::Model makeCubeModel() {
    auto vertices =
        framework::unitCube::vertices | std::views::transform([](auto position) {
            return ObjectRenderer::Vertex{
                .position = {
                    ((position.xy() / 2.f + glm::vec2(0.5f))), // Transpose xy origin to be bottom left origin
                    (position.z) / 2.f + 0.5  // Transpose z to have origin at the bottom
                },
                .textureCoordinates = position
            };
        });

    auto indices = framework::unitCube::indices;

    return {
        .vertices = {vertices.begin(), vertices.end()},
        .indices = indices
    };
}

/// Make a pillar model, It's just a cube rotated 45 degrees and squished to be taller
static ObjectRenderer::Model makePillarModel() {
    auto rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(45.f), {0.f, 0.f, 1.f});

    auto vertices =
        framework::unitCube::vertices | std::views::transform([&rotationMatrix](auto position) {
            auto textureCoordinates = position;

            // Rotate pillar
            position = glm::vec3(rotationMatrix * glm::vec4(position, 1.0));

            // Transpose z to have origin at the bottom, and scale
            position.z += 0.5f;

            // Scaling
            position *= 0.5f;
            position.z *= 2.f;

            // Transpose xy origin to be bottom left origin
            position = {
                ((position.xy() + glm::vec2(1)) / (2.f)),
                position.z
            };

            return ObjectRenderer::Vertex{
                .position = position,
                .textureCoordinates = textureCoordinates
            };
        });

    auto indices = framework::unitCube::indices;

    return {
        .vertices = {vertices.begin(), vertices.end()},
        .indices = indices
    };
}

#endif //EXAMAUTUMN2023_MODELS_H
