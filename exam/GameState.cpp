#include "GameState.h"
#include "constants.h"

void GameState::handleKeyInput(int key, int action) {
    if (action != GLFW_PRESS) return;

    switch (key) {
        // Toggle textures
        case GLFW_KEY_T:
            useTextures = !useTextures;
            break;

        case GLFW_KEY_LEFT:
            board.movePlayer(Direction::Left);
            break;
        case GLFW_KEY_RIGHT:
            board.movePlayer(Direction::Right);
            break;
        case GLFW_KEY_UP:
            board.movePlayer(Direction::Up);
            break;
        case GLFW_KEY_DOWN:
            board.movePlayer(Direction::Down);
            break;

        default:
            break;
    }
}

void GameState::update(GLFWwindow *window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_D)) {
        cameraAngle += CAMERA_SENSITIVITY * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_A)) {
        cameraAngle -= CAMERA_SENSITIVITY * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_S)) {
        cameraZoom -= ZOOM_SENSITIVITY * deltaTime;
        cameraZoom = glm::clamp(cameraZoom, MIN_ZOOM, MAX_ZOOM);
    }

    if (glfwGetKey(window, GLFW_KEY_W)) {
        cameraZoom += ZOOM_SENSITIVITY * deltaTime;
        cameraZoom = glm::clamp(cameraZoom, MIN_ZOOM, MAX_ZOOM);
    }
}
