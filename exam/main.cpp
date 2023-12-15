#include "glad/glad.h"
#include "stb_image.h"
#include "framework/window.h"
#include "framework/Camera.h"
#include "Player.h"
#include "Floor.h"

const glm::uvec2 BOARD_SIZE = {10, 10};

void startGame(GLFWwindow *window, float aspectRatio) {
    // Time
    double lastFrameTime;
    float deltaTime;

    // Camera
    glm::vec3 position = {0.f, 0.f, 30.f};
    glm::vec3 target = {0.f, 0.f, 0.f};
    glm::vec3 up = {0.f, 1.f, 0.f};

    auto camera = framework::Camera::createPerspective(45.f, aspectRatio, position, target, up);

    auto floor = Floor::create(BOARD_SIZE);
    auto player = Player::create();

    // Enable depth
    glEnable(GL_DEPTH_TEST);

    // Clear color
    glm::vec3 backgroundColor = {0.917f, 0.905f, 0.850f};

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        // Set deltaTime
        auto time = glfwGetTime();
        deltaTime = (float) (time - lastFrameTime);
        lastFrameTime = time;

        // Update
        glfwPollEvents();

        // Background color
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        floor.draw(camera);
        player.draw({9, 9}, camera);

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