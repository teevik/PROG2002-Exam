#include "BoardRenderer.h"
#include "../constants.h"

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
    cubeRenderer.draw(board.playerPosition, PLAYER_COLOR, &playerTexture, light, camera, useTextures);

    for (auto [position, objectType]: board.objects) {
        // Different color and renderer depending on object type
        switch (objectType) {
            case ObjectType::Wall:
                cubeRenderer.draw(position, WALL_COLOR, &wallTexture, light, camera, useTextures);
                break;

            case ObjectType::Pillar:
                pillarRenderer.draw(position, PILLAR_COLOR, nullptr, light, camera, useTextures);
                break;

            case ObjectType::Box: {
                const auto &storageLocations = board.storageLocations;
                auto isOnStorageLocation =
                    std::find(storageLocations.begin(), storageLocations.end(), position) != storageLocations.end();

                if (isOnStorageLocation) {
                    cubeRenderer.draw(position, FINISHED_BOX_COLOR, &boxTexture, light, camera, useTextures);
                } else {
                    cubeRenderer.draw(position, BOX_COLOR, &boxTexture, light, camera, useTextures);
                }

                break;
            }
        }
    }
}
