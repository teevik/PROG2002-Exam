#include <regex>
#include "ObjectRenderer.h"
#include "framework/geometry.h"
#include "phong_shader.h"

// language=glsl
static const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 texture_coordinates;
    layout(location = 2) in vec3 normal;

    out VertexData {
        vec3 world_position;
        vec3 texture_coordinates;
        vec3 world_normal;
    } vertex_data;

    uniform mat4 projection_matrix;
    uniform mat4 view_matrix;
    uniform mat4 model_matrix;
    uniform mat4 normal_matrix;

    void main() {
        vec4 world_position = model_matrix * vec4(position, 1);
        gl_Position = projection_matrix * view_matrix * world_position;

        vertex_data.world_position = world_position.xyz;
        vertex_data.texture_coordinates = texture_coordinates;
        vertex_data.world_normal = normalize((normal_matrix * vec4(normal, 1.0)).xyz);
    }
)";

static std::string fragmentShaderSource() {
    // language=glsl
    std::string shader = R"(
        #version 450 core

        in VertexData {
            vec3 world_position;
            vec3 texture_coordinates;
            vec3 world_normal;
        } vertex_data;

        out vec4 out_color;

        // Texture
        layout(binding=0) uniform samplerCube texture_sampler;
        uniform bool use_textures;

        // Albedo
        uniform vec3 color;

        // Camera and light
        uniform vec3 camera_position;
        uniform vec3 light_color;
        uniform vec3 light_position;

        // Material
        uniform vec3 ambient_color;
        uniform vec3 specular_color;

        // Gets replaced to include `calculate_phong`
        PHONG_SHADER_SOURCE

        void main() {
            vec4 uniform_color = vec4(color, 1);
            vec4 texture_color = texture(texture_sampler, vertex_data.texture_coordinates);

            vec4 lighting = calculate_phong(
                vertex_data.world_position,
                vertex_data.world_normal,
                camera_position,
                light_color,
                light_position,
                ambient_color,
                specular_color
            );

            vec4 albedo = mix(uniform_color, texture_color, use_textures ? 0.3 : 0);
            out_color = albedo * lighting;
        }
    )";

    // Replace `PHONG_SHADER_SOURCE` to be the `calculate_phong` function
    shader = std::regex_replace(shader, std::regex("PHONG_SHADER_SOURCE"), phongShaderSource);

    return shader;
}

std::shared_ptr<framework::Shader> ObjectRenderer::shader = nullptr;

ObjectRenderer ObjectRenderer::create() {
    if (shader == nullptr) {
        shader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource());
    }

    auto vertices =
        framework::generateUnitCubeWithNormals() | std::views::transform([](auto vertex) {
            return Vertex{
                .position = vertex.position,
                .textureCoordinates = vertex.position,
                .normal = vertex.normal
            };
        });


    auto vertexArray = framework::VertexArray(
        shader,
        ObjectRenderer::VERTEX_ATTRIBUTES,
        framework::VertexBuffer<Vertex>({vertices.begin(), vertices.end()}),
        {}
    );

    return {
        .vertexArray = std::move(vertexArray),
    };
}

void ObjectRenderer::draw(
    glm::vec2 position,
    glm::vec3 color,
    const framework::Texture *texture,
    const Light &light,
    const framework::Camera &camera,
    bool useTextures
) const {
    auto modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.f)); // 3: translate to given position
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f)); // 2: translate to make the corner of the cube origin
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f)); // 1: make smaller

    // Dont apply translation to normal matrix
    auto normalMatrix = glm::mat4(1.0f);

    shader->uploadUniformMatrix4("model_matrix", modelMatrix);
    shader->uploadUniformMatrix4("normal_matrix", normalMatrix);
    shader->uploadUniformMatrix4("projection_matrix", camera.projectionMatrix);
    shader->uploadUniformMatrix4("view_matrix", camera.viewMatrix());

    // Shading
    shader->uploadUniformFloat3("color", color);
    shader->uploadUniformFloat3("camera_position", camera.position);
    shader->uploadUniformFloat3("light_color", light.color);
    shader->uploadUniformFloat3("light_position", light.position);

    shader->uploadUniformFloat3("ambient_color", glm::vec3(0.6f));
    shader->uploadUniformFloat3("specular_color", glm::vec3(0.5f));

    shader->uploadUniformBool1("use_textures", useTextures && texture);
    if (useTextures && texture) texture->bind();

    vertexArray.draw();
}
