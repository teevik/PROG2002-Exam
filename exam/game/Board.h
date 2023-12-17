#ifndef EXAMAUTUMN2023_BOARD_H
#define EXAMAUTUMN2023_BOARD_H

#include <vector>
#include <unordered_map>
#include <optional>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec2.hpp"
#include "glm/gtx/hash.hpp"

/// Keeps track of an animation between two positions
struct Animation {
    float startTime;
    float duration;

    glm::ivec2 from;
    glm::ivec2 to;

    explicit Animation(glm::ivec2 from, glm::ivec2 to, float duration);

    [[nodiscard]] bool isFinished() const;

    [[nodiscard]] glm::vec2 currentPosition() const;
};

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

enum class ObjectType {
    Wall,
    Box,
    Pillar
};

/// An object like Wall, Box, Pillar
struct Object {
    ObjectType type;
    std::optional<Animation> animation;

    explicit Object(ObjectType type, std::optional<Animation> animation = std::nullopt);
};

/// Keeps track of the position of everything on the board
struct Board {
    const std::vector<glm::uvec2> storageLocations;
    std::unordered_map<glm::uvec2, Object> objects;
    glm::uvec2 playerPosition;
    std::optional<Animation> playerAnimation;

    static Board createInitialBoard();

    void movePlayer(Direction direction);
};

#endif //EXAMAUTUMN2023_BOARD_H
