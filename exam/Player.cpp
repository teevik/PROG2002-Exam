#include "Player.h"
#include "framework/geometry.h"

const glm::vec4 PLAYER_COLOR = {0.82, 0.89, 0.37, 1.};

Player Player::create() {
    auto shader = Object::getShader();

    auto vertices =
        framework::unitCube::vertices | std::views::transform([](auto position) {
            return Object::Vertex{
                .position = {
                    ((position.xy() + glm::vec2(1.f)) / 2.f), // Transpose xy origin to be bottom left origin
                    position.z + 0.5}, // Transpose z to have origin at the bottom
                .color = PLAYER_COLOR
            };
        });

    auto indices = framework::unitCube::indices;

    auto vertexArray = framework::VertexArray(
        shader,
        Object::VERTEX_ATTRIBUTES,
        framework::VertexBuffer<Object::Vertex>({vertices.begin(), vertices.end()}),
        framework::IndexBuffer(indices)
    );

    return {
        .shader = shader,
        .vertexArray = std::move(vertexArray)
    };
}

void Player::draw(glm::uvec2 position, const framework::Camera &camera) const {
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, {position, 0.f});

    shader->uploadUniformMatrix4("model", modelMatrix);
    shader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    shader->uploadUniformMatrix4("view", camera.viewMatrix());

    vertexArray.draw();
}
