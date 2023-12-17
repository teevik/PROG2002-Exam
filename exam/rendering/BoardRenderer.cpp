#include "BoardRenderer.h"
#include "../constants.h"
#include "GLFW/glfw3.h"

BoardRenderer BoardRenderer::create() {
    auto cubeRenderer = ObjectRenderer::create();
    auto pillarRenderer = ObjectRenderer::create();

    auto filtering = framework::Filtering::Nearest;
    auto playerTexture = framework::loadCubemap(RESOURCES_DIR + std::string("textures/player.png"), filtering);
    auto boxTexture = framework::loadCubemap(RESOURCES_DIR + std::string("textures/box.png"), filtering);
    auto wallTexture = framework::loadCubemap(RESOURCES_DIR + std::string("textures/wall.png"), filtering);

    return {
        .cubeRenderer = std::move(cubeRenderer),
        .pillarRenderer = std::move(pillarRenderer),
        .playerTexture = std::move(playerTexture),
        .boxTexture = std::move(boxTexture),
        .wallTexture = std::move(wallTexture)
    };
}

void BoardRenderer::draw(
    const Board &board,
    const Light &light,
    const framework::Camera &camera,
    bool useTextures
) const {
    // Get animated player position
    glm::vec2 playerPosition =
        board.playerAnimation.has_value() ? board.playerAnimation->currentPosition() : glm::vec2(board.playerPosition);

    // Calculate player glow
    const float GLOW_SPEED = 2.f;
    auto glowIntensity = glm::cos(glm::pi<float>() + glfwGetTime() * GLOW_SPEED); // Range [-1, 1]
    glowIntensity += 1; // Maps to range [0, 2]
    glowIntensity /= 2; // Maps to range [0, 1]
    auto playerGlow = glm::vec3(glowIntensity, glowIntensity * 0.1f, 0);

    // Render player
    cubeRenderer.draw(playerPosition, PLAYER_COLOR, &playerTexture, light, camera, useTextures, glm::vec3(playerGlow));

    // Render objects
    for (auto [staticPosition, object]: board.objects) {
        // Get animated object position
        glm::vec2 position =
            object.animation.has_value() ? object.animation->currentPosition() : glm::vec2(staticPosition);

        // Different color and renderer depending on object type
        switch (object.type) {
            case ObjectType::Wall:
                cubeRenderer.draw(position, WALL_COLOR, &wallTexture, light, camera, useTextures);
                break;

            case ObjectType::Pillar:
                pillarRenderer.draw(position, PILLAR_COLOR, nullptr, light, camera, useTextures);
                break;

            case ObjectType::Box: {
                const auto &storageLocations = board.storageLocations;
                auto storageLocation =
                    std::find(storageLocations.begin(), storageLocations.end(), staticPosition);

                // If there is a storage location on this tile, and the box position is close enough
                bool isCloseToStorageLocation =
                    storageLocation != storageLocations.end() &&
                    glm::distance(glm::vec2(*storageLocation), position) < 0.5f;

                if (isCloseToStorageLocation) {
                    cubeRenderer.draw(position, FINISHED_BOX_COLOR, &boxTexture, light, camera, useTextures);
                } else {
                    cubeRenderer.draw(position, BOX_COLOR, &boxTexture, light, camera, useTextures);
                }

                break;
            }
        }
    }
}