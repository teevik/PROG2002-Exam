#include <regex>
#include "FloorRenderer.h"
#include "../constants.h"
#include "phong_shader.h"

// language=glsl
static const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec2 position;

    out VertexData {
        vec3 world_position;
        vec3 world_normal;
        vec2 grid_position;
    } vertex_data;

    uniform mat4 projection_matrix;
    uniform mat4 view_matrix;
    uniform mat4 model_matrix;
    uniform mat4 normal_matrix;

    const vec3 NORMAL = vec3(0, 0, 1); // Normal is always up

    void main() {
        vec4 world_position = model_matrix * vec4(position, 0, 1);
        gl_Position = projection_matrix * view_matrix * world_position;

        vertex_data.world_position = world_position.xyz;
        vertex_data.world_normal = normalize((normal_matrix * vec4(NORMAL, 1.0)).xyz);
        vertex_data.grid_position = position;
    }
)";

static std::string fragmentShaderSource() {
    // language=glsl
    std::string shader = R"(
        #version 450 core

        in VertexData {
            vec3 world_position;
            vec3 world_normal;
            vec2 grid_position;
        } vertex_data;

        out vec4 color;

        // Texture
        layout(binding=0) uniform sampler2D texture_sampler;
        uniform bool use_textures;

        // Albedo
        uniform vec3 floor_color;
        uniform vec3 storage_location_color;

        // Camera and light
        uniform vec3 camera_position;
        uniform vec3 light_color;
        uniform vec3 light_position;

        // Material
        uniform vec3 ambient_color;
        uniform vec3 specular_color;

        struct StorageLocation {
            uvec2 position;
        };

        layout(std140) uniform StorageLocationBuffer {
            StorageLocation storage_locations[AMOUNT_OF_EACH_OBJECT];
        };

        const float LINE_WIDTH = 0.05;
        const float CIRCLE_SIZE = 0.2;
        const vec3 GRID_COLOR = vec3(0.2, 0.2, 0.2);

        // Gets replaced to include `calculate_phong`
        PHONG_SHADER_SOURCE

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
            vec4 chosen_color;
            if (grid_pattern) {
                chosen_color = vec4(GRID_COLOR, 1);
            } else {
                chosen_color = vec4(floor_color, 1);
            }

            vec4 texture_color = texture(texture_sampler, vertex_data.grid_position);
            chosen_color = mix(chosen_color, texture_color, use_textures ? 0.2 : 0); // Mix texture in

            // Draw circle if on storage location
            if (is_on_storage_location && circle_pattern) {
                chosen_color = vec4(storage_location_color, 1);
            }

            vec4 lighting = calculate_phong(
                vertex_data.world_position,
                vertex_data.world_normal,
                camera_position,
                light_color,
                light_position,
                ambient_color,
                specular_color
            );

            color = chosen_color * lighting;
        }
    )";

    // Replace constants with actual value
    shader = std::regex_replace(shader, std::regex("AMOUNT_OF_EACH_OBJECT"), std::to_string(AMOUNT_OF_EACH_OBJECT));

    // Replace `PHONG_SHADER_SOURCE` to be the `calculate_phong` function
    shader = std::regex_replace(shader, std::regex("PHONG_SHADER_SOURCE"), phongShaderSource);

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

    auto texture = framework::loadTexture(
        RESOURCES_DIR + std::string("textures/floor.png"),
        framework::Filtering::Nearest
    );

    return {
        .shader = shader,
        .vertexArray = std::move(vertexArray),
        .storageLocationsBuffer = std::move(storageLocationsBuffer),
        .texture = std::move(texture)
    };
}

void FloorRenderer::draw(const Light &light, const framework::Camera &camera, bool useTextures) const {
    auto modelMatrix = glm::mat4(1.0f);
    auto normalMatrix = glm::mat4(1.0f);

    shader->uploadUniformMatrix4("model_matrix", normalMatrix);
    shader->uploadUniformMatrix4("normal_matrix", modelMatrix);
    shader->uploadUniformMatrix4("projection_matrix", camera.projectionMatrix);
    shader->uploadUniformMatrix4("view_matrix", camera.viewMatrix());

    shader->uploadUniformBuffer("StorageLocationBuffer", 0, storageLocationsBuffer);

    // Shading
    shader->uploadUniformFloat3("storage_location_color", STORAGE_LOCATION_COLOR);
    shader->uploadUniformFloat3("floor_color", FLOOR_COLOR);
    shader->uploadUniformFloat3("camera_position", camera.position);
    shader->uploadUniformFloat3("light_color", light.color);
    shader->uploadUniformFloat3("light_position", light.position);

    shader->uploadUniformFloat3("ambient_color", glm::vec3(0.f));
    shader->uploadUniformFloat3("specular_color", glm::vec3(0.1f));

    shader->uploadUniformBool1("use_textures", useTextures);
    if (useTextures) texture.bind();

    vertexArray.draw();
}
