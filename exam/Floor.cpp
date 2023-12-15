#include "Floor.h"

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

// language=glsl
const std::string fragmentShaderSource = R"(
    #version 450 core

    in VertexData {
        vec2 grid_position;
    } vertex_data;

    out vec4 color;

    const float LINE_WIDTH = 0.05;

    const vec3 COLOR = vec3(1, 1, 1);
    const vec3 GRID_COLOR = vec3(0.2, 0.2, 0.2);

    void main() {
        vec2 distance_from_grid = fract(vertex_data.grid_position);

        bool vertical_line = distance_from_grid.x < LINE_WIDTH || distance_from_grid.x > (1. - LINE_WIDTH);
        bool horizontal_line = distance_from_grid.y < LINE_WIDTH || distance_from_grid.y > (1. - LINE_WIDTH);

        // If there is either a line horizontally or vertically
        bool grid = vertical_line || horizontal_line;

        color = vec4(grid ? GRID_COLOR : COLOR, 1);
    }
)";

Floor Floor::create(glm::uvec2 size) {
    auto shader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);

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
        .vertexArray = std::move(vertexArray)
    };
}

void Floor::draw(const framework::Camera &camera) const {
    auto modelMatrix = glm::mat4(1.0f);

    shader->uploadUniformMatrix4("model", modelMatrix);
    shader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    shader->uploadUniformMatrix4("view", camera.viewMatrix());

    vertexArray.draw();
}
