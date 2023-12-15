#include "ObjectRenderer.h"
#include "framework/geometry.h"

// language=glsl
const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec3 position;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main() {
        gl_Position = projection * view * model * vec4(position, 1);
    }
)";

// language=glsl
const std::string fragmentShaderSource = R"(
    #version 450 core

    out vec4 out_color;

    uniform vec3 color;

    void main() {
        out_color = vec4(color, 1);
    }
)";

std::shared_ptr<framework::Shader> ObjectRenderer::shader = nullptr;

ObjectRenderer ObjectRenderer::create(glm::vec3 color, const Model &model) {
    if (shader == nullptr) {
        shader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);
    }

//    auto vertices =
//        framework::unitCube::vertices | std::views::transform([color](auto position) {
//            return ObjectRenderer::Vertex{
//                .position = {
//                    ((position.xy() + glm::vec2(1.f)) / 2.f), // Transpose xy origin to be bottom left origin
//                    position.z + 0.5}, // Transpose z to have origin at the bottom
//            };
//        });
//
//    auto indices = framework::unitCube::indices;

    auto vertexArray = framework::VertexArray(
        shader,
        ObjectRenderer::VERTEX_ATTRIBUTES,
        framework::VertexBuffer(model.vertices),
        framework::IndexBuffer(model.indices)
    );

    return {
        .vertexArray = std::move(vertexArray),
        .color = color
    };
}

void ObjectRenderer::draw(glm::uvec2 position, const framework::Camera &camera) const {
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0));

    shader->uploadUniformMatrix4("model", modelMatrix);
    shader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    shader->uploadUniformMatrix4("view", camera.viewMatrix());

    shader->uploadUniformFloat3("color", color);

    vertexArray.draw();
}
