#include "ObjectRenderer.h"
#include "framework/geometry.h"

// language=glsl
const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 texture_coordinates;

    out VertexData {
        vec3 texture_coordinates;
    } vertex_data;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main() {
        vertex_data.texture_coordinates = texture_coordinates;
        gl_Position = projection * view * model * vec4(position, 1);
    }
)";

// language=glsl
const std::string fragmentShaderSource = R"(
    #version 450 core

    in VertexData {
        vec3 texture_coordinates;
    } vertex_data;

    out vec4 out_color;

    layout(binding=0) uniform samplerCube texture_sampler;
    uniform vec3 color;
    uniform bool use_textures;

    void main() {
        vec4 uniform_color = vec4(color, 1);
        vec4 texture_color = texture(texture_sampler, vertex_data.texture_coordinates);

        out_color = mix(uniform_color, texture_color, use_textures ? 0.3 : 0);
    }
)";

std::shared_ptr<framework::Shader> ObjectRenderer::shader = nullptr;

ObjectRenderer ObjectRenderer::create(const Model &model) {
    if (shader == nullptr) {
        shader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);
    }

    auto vertexArray = framework::VertexArray(
        shader,
        ObjectRenderer::VERTEX_ATTRIBUTES,
        framework::VertexBuffer(model.vertices),
        framework::IndexBuffer(model.indices)
    );

    return {
        .vertexArray = std::move(vertexArray),
    };
}

void ObjectRenderer::draw(
    glm::uvec2 position,
    glm::vec3 color,
    const framework::Texture *texture,
    const framework::Camera &camera,
    bool useTextures
) const {
    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0));

    shader->uploadUniformMatrix4("model", modelMatrix);
    shader->uploadUniformMatrix4("projection", camera.projectionMatrix);
    shader->uploadUniformMatrix4("view", camera.viewMatrix());

    shader->uploadUniformFloat3("color", color);

    shader->uploadUniformBool1("use_textures", useTextures && texture);
    if (useTextures && texture) texture->bind();

    vertexArray.draw();
}
