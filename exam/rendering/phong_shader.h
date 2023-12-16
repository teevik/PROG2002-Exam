#ifndef EXAMAUTUMN2023_PHONG_SHADER_H
#define EXAMAUTUMN2023_PHONG_SHADER_H

#include <string>

/// `calculate_phong` glsl function
// language=glsl
static const std::string phongShaderSource = R"(
    vec4 calculate_phong(
        vec3 world_position,
        vec3 world_normal,
        vec3 camera_position,
        vec3 light_color,
        vec3 light_position,
        vec3 ambient_color,
        vec3 specular_color
    ) {
        vec3 light_direction = normalize(vec3(light_position - world_position));

        // Ambient
        vec3 ambient = ambient_color;

        // Diffuse
        vec3 diffuse = light_color * max(dot(light_direction, world_normal), 0.0f);

        // Specular
        vec3 reflected_light = normalize(reflect(-light_direction, world_normal));
        vec3 observer_direction = normalize(camera_position - world_position);
        float specular_factor = pow(max(dot(observer_direction, reflected_light), 0.0), 12);
        vec3 specular = specular_factor * specular_color;

        return vec4((ambient + diffuse + specular), 1);
    }
)";

#endif //EXAMAUTUMN2023_PHONG_SHADER_H
