#include <regex>
#include "FloorRenderer.h"
#include "../constants.h"

// language=glsl
const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec2 position;

    out VertexData {
        vec2 grid_position;
    } vertex_data;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main() {
        vertex_data.grid_position = position;
        gl_Position = projection * view * model * vec4(position, 0, 1);
    }
)";

std::string fragmentShaderSource() {
    // language=glsl
    std::string shader = R"(
        #version 450 core

        in VertexData {
            vec2 grid_position;
        } vertex_data;

        out vec4 color;

        uniform vec3 floor_color;
        uniform vec3 storage_location_color;

        struct StorageLocation {
            uvec2 position;
        };

        layout(std140) uniform StorageLocationBuffer {
            StorageLocation storage_locations[AMOUNT_OF_EACH_OBJECT];
        };

        const float LINE_WIDTH = 0.05;
        const float CIRCLE_SIZE = 0.2;
        const vec3 GRID_COLOR = vec3(0.2, 0.2, 0.2);

        void main() {
            // Check if on a storage location tile
            bool is_on_storage_location = false;
            for (uint i = 0; i < AMOUNT_OF_EACH_OBJECT; i++) {
                vec2 storage_location = storage_locations[i].position;

                if (uvec2(vertex_data.grid_position) == storage_location) {
                    is_on_storage_location = true;
                }
            }

            // Make grid pattern
            vec2 distance_from_grid = fract(vertex_data.grid_position);
            bool vertical_line = distance_from_grid.x < LINE_WIDTH || distance_from_grid.x > (1. - LINE_WIDTH);
            bool horizontal_line = distance_from_grid.y < LINE_WIDTH || distance_from_grid.y > (1. - LINE_WIDTH);
            bool grid_pattern = vertical_line || horizontal_line;

            // Circle pattern for storage locations
            vec2 centered = (distance_from_grid * 2) - 1;
            bool circle_pattern = (centered.x * centered.x + centered.y * centered.y) < CIRCLE_SIZE;

            // Find the color to use
            vec3 chosen_color;
            if (grid_pattern) {
                chosen_color = GRID_COLOR;
            } else if (is_on_storage_location && circle_pattern) {
                chosen_color = storage_location_color;
            } else {
                chosen_color = floor_color;
            }

            color = vec4(chosen_color, 1.0);
        }
    )";

    // Replace constants with actual value
    shader = std::regex_replace(shader, std::regex("AMOUNT_OF_EACH_OBJECT"), std::to_string(AMOUNT_OF_EACH_OBJECT));

    return shader;
}


FloorRenderer FloorRenderer::create(glm::uvec2 size, const std::vector<glm::uvec2> &storageLocations) {
    auto mappedStorageLocations = storageLocations | std::views::transform([](auto storageLocation) {
        return StorageLocation{
            .position = storageLocation,
        };
    });

    auto storageLocationsBuffer = framework::UniformBuffer<StorageLocation>::create(
        {mappedStorageLocations.begin(), mappedStorageLocations.end()}
    );

    auto shader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource());

    std::vector<Vertex> vertices = {
        { // right top
            .position = {size.x, 0.f},
        },
        { // right bottom
            .position = {size.x, size.y},
        },
        { // left top
            .position = {0.f, 0.f},
        },
        { // left bottom
            .position = {0.f, size.y},
        },
    };

    std::vector<uint32_t> indices = {
        0, // right top
        1, // right bottom
        2, // left top
        1, // right bottom
        3, // left bottom
        2 // left top
    };

    auto vertexArray = framework::VertexArray(
        shader,
        VERTEX_ATTRIBUTES,
        framework::VertexBuffer(vertices),
        framework::IndexBuffer(indices)
    );

    return {
        .shader = shader,
        .vertexArray = std::move(vertexArray),
        .storageLocationsBuffer = std::move(storageLocationsBuffer)
    };
}

void FloorRenderer::draw(const framework::Camera &camera) const {
    auto modelMatrix = glm::mat4(1.0f);

    shader->uploadUniformMatrix4("model", modelMatrix);
    shader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    shader->uploadUniformMatrix4("view", camera.viewMatrix());

    shader->uploadUniformFloat3("floor_color", FLOOR_COLOR);
    shader->uploadUniformFloat3("storage_location_color", STORAGE_LOCATION_COLOR);
    shader->uploadUniformBuffer("StorageLocationBuffer", 0, storageLocationsBuffer);

    vertexArray.draw();
}
