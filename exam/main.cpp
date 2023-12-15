#include "glad/glad.h"
#include "stb_image.h"
#include "framework/window.h"
#include "framework/Camera.h"
#include "Object.h"
#include "Floor.h"
#include "data_structures/SparseGrid.h"

const glm::uvec2 BOARD_SIZE = {10, 10};
const glm::vec3 PLAYER_COLOR = {0.82, 0.89, 0.37};
const glm::vec3 WALL_COLOR = {0.41, 0.37, 0.30};

SparseGrid<Object> createWalls() {
    auto walls = SparseGrid<Object>(BOARD_SIZE);

    auto createWall = [&walls](glm::vec2 position) {
        auto wall = Object::create(WALL_COLOR);
        walls.set(position, std::move(wall));
    };

    for (uint32_t x = 0; x < BOARD_SIZE.x; x++) {
        createWall({x, 0});
        createWall({x, BOARD_SIZE.y - 1});
    }

    // Makes some duplicates, but they will just replace the overlapping walls in the grid
    for (uint32_t y = 0; y < BOARD_SIZE.y; y++) {
        createWall({0, y});
        createWall({BOARD_SIZE.x - 1, y});
    }

    return std::move(walls);
}

void startGame(GLFWwindow *window, float aspectRatio) {
    // Time
    double lastFrameTime;
    float deltaTime;

    // Camera
    float angle = 0;
    glm::vec2 rotationOffset = glm::vec2(glm::cos(angle), glm::sin(angle)) * 8.f;
    glm::vec2 centerOfBoard = glm::vec2(BOARD_SIZE) / 2.f;

    glm::vec3 position = {centerOfBoard + rotationOffset, 20.f};
    glm::vec3 target = {centerOfBoard, 0.f};
    glm::vec3 up = {0.f, 0.f, 1.f};

    auto camera = framework::Camera::createPerspective(45.f, aspectRatio, position, target, up);

    auto floor = Floor::create(BOARD_SIZE);
    auto player = Object::create(PLAYER_COLOR);
    auto walls = createWalls();

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
        player.draw({1, 1}, camera);

        for (uint32_t x = 0; x < BOARD_SIZE.x; x++) {
            for (uint32_t y = 0; y < BOARD_SIZE.y; y++) {
                auto &wall = walls.get({x, y});

                if (wall.has_value()) wall->draw({x, y}, camera);
            }
        }

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