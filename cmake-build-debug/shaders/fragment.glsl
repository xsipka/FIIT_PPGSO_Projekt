#version 400

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D texture;
};

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

// number of lights
const int LIGHTS_NUM = 5;

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texture_coord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;
uniform Light lights[LIGHTS_NUM];
uniform vec3 camera_pos;

// used for calculating attenuation
const float CONST = 1.5f;
const float LINEAR = 0.045f;
const float QUAD = 0.25f;   // increase for darkness


// calculates ambient light
vec3 get_ambient(Material material) {
    return material.ambient;
}

// calculates diffuse ligh
vec3 get_diffuse(Material material, vec3 vs_position, vec3 vs_normal, Light light) {

    vec3 light_dir = normalize(light.position - vs_position);
    float diffuse = clamp(dot(light_dir, normalize(vs_normal)), 1.f, 0.5f);
    vec3 diffuse_final =  material.diffuse * diffuse;

    return diffuse_final;
}

// calculates specular light
vec3 get_specular(Material material, vec3 vs_position, vec3 vs_normal, Light light, vec3 camera_pos) {

    vec3 specular_vec = normalize(vs_position - light.position);
    vec3 reflection = normalize(reflect(specular_vec, normalize(vs_normal)));
    vec3 view_pos = normalize(camera_pos - vs_position);
    float specularity = pow(max(dot(view_pos, reflection), 0), 50);
    vec3 specular_final = material.specular  * specularity;

    return specular_final;
}

// calculates attenuation
float get_attenuation(Light light, vec3 vs_position) {

    float distance = length(light.position - vs_position);
    float attenuation = CONST / (QUAD * pow(distance, 2) + LINEAR * distance + 1.f);

    return attenuation;
}

// calculates point light
vec4 get_point_light(Material material, Light light, vec2 vs_texture_coord,
                    vec3 ambient, vec3 diffuse, vec3 specular, float attenuation) {

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec4 result = texture(material.texture, vs_texture_coord) * vec4(light.color, 1.f) *
                  (vec4(ambient, 1.f) + vec4(diffuse, 1.f) + vec4(specular, 1.f));;
    return result;
}


void main() {

    vec4 temp;

    for (int i = 0; i < LIGHTS_NUM; i++) {

        // Ambient, diffuse & specular calculations
        vec3 ambient_final = get_ambient(material);
        vec3 diffuse_final = get_diffuse(material, vs_position, vs_normal, lights[i]);
        vec3 specular_final = get_specular(material, vs_position, vs_normal, lights[i], camera_pos);

        // Applying attenuation
        float attenuation = get_attenuation(lights[i], vs_position);

        // Mixing it all and creating point lights
        temp += get_point_light(material, lights[i], vs_texture_coord,
        ambient_final, diffuse_final, specular_final, attenuation);
    }

    fs_color = temp;
}
