#ifndef EXAMAUTUMN2023_CONSTANTS_H
#define EXAMAUTUMN2023_CONSTANTS_H

#include "glm/fwd.hpp"
#include "glm/detail/type_vec2.hpp"
#include "glm/detail/type_vec3.hpp"

// Game
const glm::uvec2 BOARD_SIZE = {10, 10};
const uint32_t AMOUNT_OF_EACH_OBJECT = 6;
const float ANIMATION_DURATION = 0.15f;

// Camera
const float CAMERA_SENSITIVITY = 1.75f;
const float ZOOM_SENSITIVITY = 1.f;
const float MIN_ZOOM = 0.6f;
const float MAX_ZOOM = 1.5f;

// Colors
const glm::vec3 BACKGROUND_COLOR = {0.843, 0.831, 0.796}; // #E8E5D8
const glm::vec3 FLOOR_COLOR = {0.969, 0.945, 0.843}; // #F7F1D7

const glm::vec3 PLAYER_COLOR = {0.886, 0.416, 0.29};
const glm::vec3 WALL_COLOR = {0.275, 0.247, 0.208};
const glm::vec3 BOX_COLOR = {0.812, 0.698, 0.51};
const glm::vec3 FINISHED_BOX_COLOR = {0.404, 0.329, 0.196};
const glm::vec3 STORAGE_LOCATION_COLOR = {0.667, 0.588, 0.463};
const glm::vec3 PILLAR_COLOR = {0.204, 0.2, 0.169}; // #34332B

#endif //EXAMAUTUMN2023_CONSTANTS_H
