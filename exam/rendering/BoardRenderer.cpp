#include "BoardRenderer.h"
#include "../models.h"
#include "../constants.h"

BoardRenderer BoardRenderer::create() {
    auto cubeModel = makeCubeModel();
    auto pillarModel = makePillarModel();

    auto cubeRenderer = ObjectRenderer::create(cubeModel);
    auto pillarRenderer = ObjectRenderer::create(pillarModel);

    return {
        .cubeRenderer = std::move(cubeRenderer),
        .pillarRenderer = std::move(pillarRenderer)
    };
}

void BoardRenderer::draw(const Board &board, const framework::Camera &camera) const {
    cubeRenderer.draw(board.playerPosition, PLAYER_COLOR, camera);

    for (auto [position, objectType]: board.objects) {
        // Different color and renderer depending on object type
        switch (objectType) {
            case ObjectType::Wall:
                cubeRenderer.draw(position, WALL_COLOR, camera);
                break;

            case ObjectType::Pillar:
                pillarRenderer.draw(position, PILLAR_COLOR, camera);
                break;

            case ObjectType::Box: {
                const auto &storageLocations = board.storageLocations;
                auto isOnStorageLocation =
                    std::find(storageLocations.begin(), storageLocations.end(), position) != storageLocations.end();

                if (isOnStorageLocation) {
                    cubeRenderer.draw(position, FINISHED_BOX_COLOR, camera);
                } else {
                    cubeRenderer.draw(position, BOX_COLOR, camera);
                }

                break;
            }
        }
    }
}
