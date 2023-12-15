#include "glad/glad.h"
#include "stb_image.h"
#include "framework/window.h"
#include "framework/Camera.h"
#include "ObjectRenderer.h"
#include "Floor.h"
#include "constants.h"
#include <algorithm>
#include <random>
#include "glm/gtx/hash.hpp"

enum class ObjectType {
    Wall,
    Box,
    Pillar
};

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

struct Board {
    std::unordered_map<glm::uvec2, ObjectType> objects;
    glm::uvec2 playerPosition;
};

Board createInitialBoard() {
    auto objects = std::unordered_map<glm::uvec2, ObjectType>();

    //
    for (uint32_t x = 0; x < BOARD_SIZE.x; x++) {
        objects.insert({{x, 0}, ObjectType::Wall});
        objects.insert({{x, BOARD_SIZE.y - 1}, ObjectType::Wall});
    }
    for (uint32_t y = 0; y < BOARD_SIZE.y; y++) {
        objects.insert({{0, y}, ObjectType::Wall});
        objects.insert({{BOARD_SIZE.x - 1, y}, ObjectType::Wall});
    }

    auto shuffledBoardPositions = getShuffledBoardPositions();

    auto nextRandomPosition = [&shuffledBoardPositions]() {
        auto value = shuffledBoardPositions.back();
        shuffledBoardPositions.pop_back();

        return value;
    };

    for (int i = 0; i < AMOUNT_OF_EACH_OBJECT; i++) {
        auto boxPosition = nextRandomPosition();
        objects.insert({boxPosition, ObjectType::Box});

        auto pillarPosition = nextRandomPosition();
        objects.insert({pillarPosition, ObjectType::Pillar});
    }

    auto playerPosition = nextRandomPosition();

    return {.objects = objects, .playerPosition = playerPosition};
}

void startGame(GLFWwindow *window, float aspectRatio) {
    // Time
    double lastFrameTime;
    float deltaTime;

    // Camera
    float angle = 0;
    glm::vec2 rotationOffset = glm::vec2(glm::cos(angle), glm::sin(angle)) * 8.f;
    glm::vec2 centerOfBoard = glm::vec2(BOARD_SIZE) / 2.f;

    glm::vec3 cameraPosition = {centerOfBoard + rotationOffset, 20.f};
    glm::vec3 cameraTarget = {centerOfBoard, 0.f};
    glm::vec3 cameraUp = {0.f, 0.f, 1.f};

    auto camera = framework::Camera::createPerspective(45.f, aspectRatio, cameraPosition, cameraTarget, cameraUp);

    // Rendering
    auto floor = Floor::create(BOARD_SIZE);
    auto playerRenderer = ObjectRenderer::create(PLAYER_COLOR);
    auto wallRenderer = ObjectRenderer::create(WALL_COLOR);
    auto boxRenderer = ObjectRenderer::create(BOX_COLOR);
    auto pillarRenderer = ObjectRenderer::create(PILLAR_COLOR);

    // Game state
    auto board = createInitialBoard();

    // Enable depth
    glEnable(GL_DEPTH_TEST);

    // Event loop
    while (!glfwWindowShouldClose(window)) {
        // Set deltaTime
        auto time = glfwGetTime();
        deltaTime = (float) (time - lastFrameTime);
        lastFrameTime = time;

        // Update
        glfwPollEvents();

        // Background color
        glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 1.0f);

        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        floor.draw(camera);
        playerRenderer.draw(board.playerPosition, camera);

        for (auto [position, objectType]: board.objects) {
            switch (objectType) {
                case ObjectType::Wall:
                    wallRenderer.draw(position, camera);
                    break;

                case ObjectType::Box:
                    boxRenderer.draw(position, camera);
                    break;

                case ObjectType::Pillar:
                    pillarRenderer.draw(position, camera);
                    break;
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