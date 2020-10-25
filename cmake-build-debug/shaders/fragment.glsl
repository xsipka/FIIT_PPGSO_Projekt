#version 400

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuse_texture;
    sampler2D specular_texture;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texture_coord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;
uniform PointLight point_light;
uniform vec3 camera_pos;

void main() {

    // Diffuse light              to          from
    vec3 diffuse_vec = normalize(point_light.position - vs_position);
    float diffuse = clamp(dot(diffuse_vec, normalize(vs_normal)), 0, 1);
    vec3 diffuse_final =  material.diffuse * diffuse;

    // Specular light - shiny effect
    vec3 specular_vec = normalize(vs_position - point_light.position);
    vec3 reflection = normalize(reflect(specular_vec, normalize(vs_normal)));
    vec3 view_pos = normalize(camera_pos - vs_position);
    float specularity = pow(max(dot(view_pos, reflection), 0), 50);
    vec3 specular_final = material.specular  * specularity /** texture(material.specular_texture, vs_texture_coord).rgb*/;

    // Attenuation
    float distance = length(point_light.position - vs_position);
    float attenuation = 1.f / (0.075f * pow(distance, 2) + 0.045f * distance + 1.f);

    // Ambient light
    vec3 ambient = material.ambient * attenuation;

    // Adding attenuation
    specular_final *= attenuation;
    diffuse_final *= attenuation;

    // Final
    //fs_color = texture(material.diffuse_texture, vs_texture_coord) * texture(material.specular_texture, vs_texture_coord) *
    //vec4(vs_color, 1.f) * (vec4(ambient, 1.f) + vec4(diffuse_final, 1.f) + vec4(specular_final, 1.f));

    fs_color = vec4(point_light.color, 1.f) * (vec4(ambient, 1.f) + vec4(diffuse_final, 1.f) + vec4(specular_final, 1.f));
}
