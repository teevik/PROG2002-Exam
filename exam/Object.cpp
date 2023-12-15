#include "Object.h"

// language=glsl
const std::string vertexShaderSource = R"(
    #version 450 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec4 color;

    out VertexData {
        vec4 color;
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
        vec4 color;
    } vertex_data;

    out vec4 color;

    void main() {
        color = vertex_data.color;
    }
)";

std::shared_ptr<framework::Shader> Object::shader = nullptr;

std::shared_ptr<framework::Shader> Object::getShader() {
    if (shader == nullptr) {
        shader = std::make_shared<framework::Shader>(vertexShaderSource, fragmentShaderSource);
    }

    return shader;
}
