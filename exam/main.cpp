#include "glad/glad.h"
#include "stb_image.h"
#include "framework/window.h"
#include "framework/Camera.h"
#include "constants.h"
#include <random>
#include "rendering/FloorRenderer.h"
#include "game/Board.h"
#include "GameState.h"
#include "rendering/BoardRenderer.h"

framework::Camera createCamera(float aspectRatio) {
    glm::vec2 centerOfBoard = glm::vec2(BOARD_SIZE) / 2.f;
    glm::vec3 cameraTarget = {centerOfBoard, 0.f};
    glm::vec3 cameraUp = {0.f, 0.f, 1.f};

    // Dont need position here since we set it in render loop
    return framework::Camera::createPerspective(45.f, aspectRatio, {}, cameraTarget, cameraUp);
}

glm::vec3 getCameraPosition(float angle, float zoom) {
    glm::vec3 position = glm::vec3(10.f * glm::cos(angle), 10.f * glm::sin(angle), 16.f);
    position *= zoom;

    glm::vec2 centerOfBoard = glm::vec2(BOARD_SIZE) / 2.f;

    return position + glm::vec3(centerOfBoard, 0.f);
}

/// Handles starting and running the game, ensures all resources are cleaned up before exiting.
void startGame(GLFWwindow *window, float aspectRatio) {
    // Time
    double lastFrameTime;
    float deltaTime;

    // Game state, needs to be static to access it from `handleKeyInput`
    static auto gameState = GameState{
        .cameraAngle = glm::pi<float>() * 1.5f,
        .cameraZoom = 1.f,
        .useTextures = true,
        .board = Board::createInitialBoard()
    };

    // Handle input
    auto handleKeyInput = [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        gameState.handleKeyInput(key, action);
    };
    glfwSetKeyCallback(window, handleKeyInput);

    // Camera
    auto camera = createCamera(aspectRatio);

    // Rendering
    glEnable(GL_DEPTH_TEST);
    auto floorRenderer = FloorRenderer::create(BOARD_SIZE, gameState.board.storageLocations);
    auto boardRenderer = BoardRenderer::create();

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Set deltaTime
        auto time = glfwGetTime();
        deltaTime = (float) (time - lastFrameTime);
        lastFrameTime = time;

        // Update
        glfwPollEvents();
        gameState.update(window, deltaTime);
        camera.position = getCameraPosition(gameState.cameraAngle, gameState.cameraZoom);

        // Background color
        glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 1.0f);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        floorRenderer.draw(camera);
        boardRenderer.draw(gameState.board, camera);

        // Swap front and back buffer
        glfwSwapBuffers(window);

        // Escape button
        bool isPressingEscape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (isPressingEscape) break;
    }
}

int main() {
    int width = 1024;
    int height = 1024;
    float aspectRatio = (float) width / (float) height;

    auto window = framework::createWindow(width, height, "Exam");
    startGame(window, aspectRatio);

    glfwTerminate();
    return EXIT_SUCCESS;
}