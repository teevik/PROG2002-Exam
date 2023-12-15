#ifndef EXAMAUTUMN2023_MODELS_H
#define EXAMAUTUMN2023_MODELS_H

#include <vector>
#include "ObjectRenderer.h"
#include "framework/geometry.h"

static ObjectRenderer::Model makeCubeModel() {
    auto vertices =
        framework::unitCube::vertices | std::views::transform([](auto position) {
            return ObjectRenderer::Vertex{
                .position = {
                    ((position.xy() + glm::vec2(1.f)) / 2.f), // Transpose xy origin to be bottom left origin
                    position.z + 0.5}, // Transpose z to have origin at the bottom
            };
        });

    auto indices = framework::unitCube::indices;

    return {
        .vertices = {vertices.begin(), vertices.end()},
        .indices = indices
    };
}

static ObjectRenderer::Model makePillarModel() {
    auto transformationMatrix = glm::mat4(1.0f);
    transformationMatrix = glm::rotate(transformationMatrix, glm::radians(45.f), {0.f, 0.f, 1.f});

    auto vertices =
        framework::unitCube::vertices | std::views::transform([&transformationMatrix](auto position) {
            // Rotate pillar
            position = glm::vec3(transformationMatrix * glm::vec4(position, 1.0));
            
            // Transpose z to have origin at the bottom, and scale
            position.z += 0.5f;

            position *= 0.5f;
            position.z *= 2.5f;

            // Transpose xy origin to be bottom left origin
            position = {
                ((position.xy() + glm::vec2(1)) / (2.f)),
                position.z
            };

            return ObjectRenderer::Vertex{
                .position = position
            };
        });

    auto indices = framework::unitCube::indices;

    return {
        .vertices = {vertices.begin(), vertices.end()},
        .indices = indices
    };
}

#endif //EXAMAUTUMN2023_MODELS_H
