#include <random>
#include <algorithm>
#include "Board.h"
#include "../constants.h"

/// Helper list for finding random positions without picking the same one again
std::vector<glm::uvec2> getShuffledBoardPositions() {
    std::vector<glm::uvec2> shuffledBoardPositions;

    // Add all positions that arent a wall
    for (uint32_t x = 1; x < (BOARD_SIZE.x - 1); x++) {
        for (uint32_t y = 1; y < (BOARD_SIZE.y - 1); y++) {
            shuffledBoardPositions.emplace_back(x, y);
        }
    }

    // Shuffle vector
    std::random_device rd;
    std::shuffle(shuffledBoardPositions.begin(), shuffledBoardPositions.end(), std::default_random_engine(rd()));

    return shuffledBoardPositions;
}

Board Board::createInitialBoard() {
    auto objects = std::unordered_map<glm::uvec2, ObjectType>();

    // Add all walls
    for (uint32_t x = 0; x < BOARD_SIZE.x; x++) {
        objects.insert({{x, 0}, ObjectType::Wall});
        objects.insert({{x, BOARD_SIZE.y - 1}, ObjectType::Wall});
    }
    for (uint32_t y = 0; y < BOARD_SIZE.y; y++) {
        objects.insert({{0, y}, ObjectType::Wall});
        objects.insert({{BOARD_SIZE.x - 1, y}, ObjectType::Wall});
    }

    // Helper list for finding random positions without picking the same one again
    auto shuffledBoardPositions = getShuffledBoardPositions();
    auto nextRandomPosition = [&shuffledBoardPositions]() {
        auto value = shuffledBoardPositions.back();
        shuffledBoardPositions.pop_back();

        return value;
    };

    std::vector<glm::uvec2> storageLocations;

    // Insert all objects
    for (int i = 0; i < AMOUNT_OF_EACH_OBJECT; i++) {
        auto boxPosition = nextRandomPosition();
        objects.insert({boxPosition, ObjectType::Box});

        auto storageLocation = nextRandomPosition();
        storageLocations.push_back(storageLocation);

        auto pillarPosition = nextRandomPosition();
        objects.insert({pillarPosition, ObjectType::Pillar});
    }

    auto playerPosition = nextRandomPosition();

    return {
        .storageLocations = storageLocations,
        .objects = objects,
        .playerPosition = playerPosition
    };
}

static glm::ivec2 directionToVector(Direction direction) {
    switch (direction) {
        case Direction::Up:
            return {0, 1};
        case Direction::Down:
            return {0, -1};
        case Direction::Left:
            return {-1, 0};
        case Direction::Right:
            return {1, 0};
    }

    return {}; // Unreachable
}

static bool isInBounds(glm::ivec2 position) {
    return
        position.x >= 0 &&
        position.x < BOARD_SIZE.x &&
        position.y >= 0 &&
        position.y < BOARD_SIZE.y;
}

void Board::movePlayer(Direction direction) {
    auto directionVec = directionToVector(direction);

    auto wantedPlayerPosition = glm::ivec2(playerPosition) + directionVec;

    if (!objects.contains(wantedPlayerPosition)) {
        // No collision
        playerPosition = wantedPlayerPosition;
    } else if (objects.at(wantedPlayerPosition) == ObjectType::Box) {
        // Collision with box
        auto positionBehindObstacle = wantedPlayerPosition + directionVec;

        if (isInBounds(positionBehindObstacle) && !objects.contains(positionBehindObstacle)) {
            // Remove box from old position
            objects.erase(wantedPlayerPosition);
            // Place box where it would be pushed to
            objects.insert({positionBehindObstacle, ObjectType::Box});

            playerPosition = wantedPlayerPosition;
        }
    }
}
