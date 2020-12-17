#include <vector>


class Model {
private:
    Material* m_material;
    Texture* m_texture;
    std::vector<Mesh*> m_meshes;
    glm::vec3 m_position;

public:
    // Constructor
    Model(Material* material, Texture* texture,
          const std::vector<Mesh*>& meshes, glm::vec3 position) {

        m_material = material;
        m_texture = texture;
        m_position = position;

        for (auto* i : meshes) {
            m_meshes.push_back(new Mesh(*i));
        }

        for (auto& i : m_meshes) {
            i->move_mesh(m_position);
        }
    }

    // Destructor
    ~Model() = default;


    void rotate_model(glm::vec3 rotation) {
        for (auto& i : m_meshes) {
            i->rotate_mesh(rotation);
        }
    }

    void move_model(glm::vec3 shift) {
        for (auto& i : m_meshes) {
            i->move_mesh(shift);
        }
    }

    void set_position(glm::vec3 position) {
        //m_position = position;
        for (auto& i : m_meshes) {
            i->set_position(position);
        }
    }

    void set_rotation(glm::vec3 rotation) {
        for (auto& i : m_meshes) {
            i->set_rotation(rotation);
        }
    }

    void scale_model(glm::vec3 scale) {
        for (auto& i : m_meshes) {
            i->set_scale(scale);
        }
    }

    void rotate_around(glm::vec3 position) {
        for (auto& i : m_meshes) {
            i->rotate_around(position);
        }
    }


    glm::vec3 get_position() {
        return m_meshes[0]->get_position();
    }

    glm::vec3 get_rotation() {
        return m_meshes[0]->get_rotation();
    }

    void render_model(Shader* shader) {

        m_material->send_to_shader(*shader);
        shader->use_program();

        // Activate texture
        m_texture->bind_texture(GL_TEXTURE_2D, 0);

        // Draw
        for (auto& i : m_meshes) {
            i->render_mesh(shader);
        }
    }
};