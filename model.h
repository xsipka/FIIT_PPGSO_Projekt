#include <vector>


class Model {
private:
    Material* m_material;
    Texture* m_diffuse_tex;
    Texture* m_specular_tex;
    std::vector<Mesh*> m_meshes;
    glm::vec3 m_position;


    void update_model_uniforms() {

    }

public:
    // Constructor
    Model(Material* material, Texture* diffuse_tex, Texture* specular_tex,
          const std::vector<Mesh*>& meshes, glm::vec3 position) {

        m_material = material;
        m_diffuse_tex = diffuse_tex;
        m_specular_tex = specular_tex;
        m_position = position;

        for (auto* i : meshes) {
            m_meshes.push_back(new Mesh(*i));
        }

        for (auto& i : m_meshes) {
            i->move_mesh(m_position);
        }
    }

    // Destructor
    ~Model() {
        for (auto*& i : m_meshes) {
            delete i;
        }
    }


    void rotate_model(glm::vec3 rotation) {
        for (auto& i : m_meshes) {
            i->rotate_mesh(rotation);
        }
    }

    glm::vec3 get_position() {
        return m_position;
    }

    void render_model(Shader* shader) {
        update_model_uniforms();
        m_material->send_to_shader(*shader);
        shader->use_program();

        // Activate texture
        m_diffuse_tex->bind_texture(GL_TEXTURE_2D, 0);
        m_specular_tex->bind_texture(GL_TEXTURE_2D, 1);

        // Draw
        for (auto& i : m_meshes) {
            i->render(shader);
        }
    }
};