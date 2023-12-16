#ifndef EXAMAUTUMN2023_GAMESTATE_H
#define EXAMAUTUMN2023_GAMESTATE_H

#include "GLFW/glfw3.h"
#include "Board.h"

/// Handles game state and input, contains no rendering code.
struct GameState {
    /// Camera angle
    float cameraAngle;

    /// Camera zoom
    float cameraZoom;

    /// Whether to render with textures or not
    bool useTextures;

    /// Game board
    Board board;

    /// Handle key input from GLFW
    void handleKeyInput(int key, int action);

    /// Game loop update
    void update(GLFWwindow *window, float deltaTime);
};

#endif //EXAMAUTUMN2023_GAMESTATE_H
