#include "Object.h"
#include "framework/geometry.h"

// language=glsl
const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 color;

    out VertexData {
        vec3 color;
    } vertex_data;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main() {
        vertex_data.color = color;

        gl_Position = projection * view * model * vec4(position, 1.0);
    }
)";

// language=glsl
const std::string fragmentShaderSource = R"(
    #version 450 core

    in VertexData {
        vec3 color;
    } vertex_data;

    out vec4 color;

    void main() {
        color = vec4(vertex_data.color, 1);
    }
)";

std::shared_ptr<framework::Shader> Object::shader = nullptr;

Object Object::create(glm::vec3 color) {
    if (shader == nullptr) {
        shader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);
    }

    auto vertices =
        framework::unitCube::vertices | std::views::transform([color](auto position) {
            return Object::Vertex{
                .position = {
                    ((position.xy() + glm::vec2(1.f)) / 2.f), // Transpose xy origin to be bottom left origin
                    position.z + 0.5}, // Transpose z to have origin at the bottom
                .color = color
            };
        });

    auto indices = framework::unitCube::indices;

    auto vertexArray = framework::VertexArray(
        shader,
        Object::VERTEX_ATTRIBUTES,
        framework::VertexBuffer<Object::Vertex>({vertices.begin(), vertices.end()}),
        framework::IndexBuffer(indices)
    );

    return Object(std::move(vertexArray));
}

Object::Object(framework::VertexArray<Object::Vertex> vertexArray) : vertexArray(std::move(vertexArray)) {}

// Move constructor
Object::Object(Object &&object) noexcept: vertexArray(std::move(object.vertexArray)) {}

void Object::draw(glm::uvec2 position, const framework::Camera &camera) const {
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, {position, 0.f});

    shader->uploadUniformMatrix4("model", modelMatrix);
    shader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    shader->uploadUniformMatrix4("view", camera.viewMatrix());

    vertexArray.draw();
}
