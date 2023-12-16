#ifndef EXAMAUTUMN2023_BOARD_H
#define EXAMAUTUMN2023_BOARD_H

#include <vector>
#include <unordered_map>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec2.hpp"
#include "glm/gtx/hash.hpp"

enum class ObjectType {
    Wall,
    Box,
    Pillar
};

struct Board {
    const std::vector<glm::uvec2> storageLocations;
    std::unordered_map<glm::uvec2, ObjectType> objects;
    glm::uvec2 playerPosition;

    static Board createInitialBoard();
};

#endif //EXAMAUTUMN2023_BOARD_H
