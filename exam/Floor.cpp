#include "Floor.h"

// language=glsl
const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec2 position;
    layout(location = 1) in uvec2 grid_position;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main() {
        gl_Position = projection * view * model * vec4(position, 0, 1);
    }
)";

// language=glsl
const std::string fragmentShaderSource = R"(
    #version 450 core

    out vec4 color;

    void main() {
        color = vec4(1, 1, 1, 1);
    }
)";

Floor Floor::create(glm::uvec2 size) {
    auto shader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);

    std::vector<Vertex> vertices = {
        { // right top
            .position = {size.x, size.y},
            .gridPosition = {size.x, size.y}
        },
        { // right bottom
            .position = {size.x, 0.f},
            .gridPosition = {size.x, 0}
        },
        { // left top
            .position = {0.f, size.y},
            .gridPosition = {0, size.y}
        },
        { // left bottom
            .position = {0.f, 0.f},
            .gridPosition = {0, 0}
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
        .size = size,
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
