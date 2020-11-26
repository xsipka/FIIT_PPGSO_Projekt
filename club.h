#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Confetti {
private:
    Texture* m_texture{};
    std::vector<Mesh*> m_shape;
    Material* m_material{};
    Model* m_model{};

    glm::vec3 m_speed;
    glm::vec3 m_position;
    float m_time_to_live{};

    const float CONF_WIDTH = 0.001;
    const float CONF_LENGTH = 0.05;
    const float GRAVITY = 9.81f;
    const float FLOOR = -0.745f;

    static float get_random(float min, float max) {

        auto num = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
        return num;
    }

    static glm::vec3 randomize_position() {

        auto position = glm::vec3(0.f);
        position.x = get_random(-5.f, 5.f);
        position.y = 3.75f;
        position.z = get_random(-5.f, 5.f);

        return position;
    }

    glm::vec3 randomize_speed() const {

        auto speed = glm::vec3(0.f);
        speed.x = get_random(0, 50) * 0.5f;
        speed.y = -GRAVITY;
        speed.z = get_random(0, 50);

        auto chance = get_random(0, 100);
        if (chance < 50) {
            speed.x *= -1;
            speed.z *= -1;
        }
        return speed;
    }

    void confetti_init() {

        m_position = randomize_position();
        m_speed = glm::vec3(0.f);
        m_time_to_live = 15.f;

        m_texture = new Texture(R"(images\confetti_01.png)", GL_TEXTURE_2D);
        m_shape.push_back(new Mesh(Cube().get_vertices(),  Cube().get_vertices_num(),
                                   Cube().get_indices(), Cube().get_indices_num()));

        m_material = new Material(glm::vec3(0.5f), glm::vec3(1.f), glm::vec3(10.f), 0);
        m_model = new Model(m_material, m_texture, m_shape, m_position);
        m_model->scale_model(glm::vec3(CONF_LENGTH, CONF_WIDTH, CONF_LENGTH));
    }

    void wall_collisions() {

        if (m_position.x > 5.45f) {
            m_position.x -= 0.05f;
        }
        if (m_position.x < -5.45f) {
            m_position.x += 0.05f;
        }
        if (m_position.z > 5.45f) {
            m_position.z -= 0.05f;
        }
        if (m_position.x < -5.45f) {
            m_position.x += 0.05f;
        }
    }

public:
    // Constructor
    Confetti() {
        confetti_init();
    }

    // Destructor
    ~Confetti() {

        delete m_texture;
        delete m_material;

        for (auto& i : m_shape) {
            delete i;
        }

        delete m_model;
    }

    bool update(float delta_time) {

        m_time_to_live -= delta_time;
        if (m_time_to_live < 0) {
            return false;
        }
        m_position += m_speed * delta_time * 0.25f;
        wall_collisions();

        // Confetti staying on the ground
        if (m_position.y <= FLOOR) {
            return true;
        }

        m_model->move_model(m_speed * delta_time * 0.25f);
        m_speed += randomize_speed() * delta_time;

        return true;
    }

    void render(Shader* shader) {
        m_model->render_model(shader);
    }

};



class Club {
private:
    std::vector<Model*> m_models;

    Texture* m_wall_tex{};
    Texture* m_floor_tex{};

    Material* m_wall_mat{};
    Material* m_floor_mat{};

    std::vector<Mesh*> m_floor_mesh;
    std::vector<Mesh*> m_wall_mesh;

    std::vector<Lightning*> m_lights;

    std::vector<Confetti*> m_confetti;

    // Load textures
    void textures_init() {
        m_floor_tex = new Texture(R"(images\floor_01.png)", GL_TEXTURE_2D);
        m_wall_tex = new Texture(R"(images\wall_01.jpg)", GL_TEXTURE_2D);
    }

    // Create materials
    void material_init() {
        m_floor_mat = new Material(glm::vec3(0.25f), glm::vec3(1.f), glm::vec3(3.5f), 0);
        m_wall_mat =  new Material(glm::vec3(0.35f), glm::vec3(1.f), glm::vec3(0.0f), 0);
    }

    // Create meshes
    void meshes_init() {
        // Floor & ceiling meshes
        m_floor_mesh.push_back(new Mesh (Square().get_vertices(),  Square().get_vertices_num(),
                                         Square().get_indices(), Square().get_indices_num()));
        m_floor_mesh[0]->scale_mesh(glm::vec3(10.f));

        // Wall meshes
        m_wall_mesh.push_back(new Mesh (Cube().get_vertices(),  Cube().get_vertices_num(),
                                        Cube().get_indices(), Cube().get_indices_num()));
        m_wall_mesh[0]->scale_mesh(glm::vec3(10.f));

        //std::vector<Vertex> temp;
        //temp = load_obj_file("obj_files/bar_03.obj");
        //m_wall_mesh.push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));
    }

    // Put everything together and create whole club
    void models_init() {
        m_models.push_back(new Model(m_floor_mat, m_floor_tex, m_floor_mesh, glm::vec3(0.f, -0.75f, 0.f)));
        m_models.push_back(new Model(m_wall_mat, m_wall_tex, m_floor_mesh, glm::vec3(0.f, 3.75f, 0.f)));
        m_models.push_back(new Model(m_wall_mat, m_wall_tex, m_wall_mesh, glm::vec3(0.f, 0.f, 0.f)));
    }

    // Create lightning
    void lights_init() {
        m_lights.push_back(new Lightning(glm::vec3 (4.5f, 3.f, 5.5f),  glm::vec3 (0.2f, 0.f, 0.5f), 10.f, 0));
        m_lights.push_back(new Lightning(glm::vec3 (-4.5f, 3.f, 5.5f), glm::vec3 (0.2f, 0.f, 0.5f), 10.f, 1));

        // Backwall lights
        m_lights.push_back(new Lightning(glm::vec3 (4.5f, 3.f, -3.5f), glm::vec3 (0.6f, 0.f, 0.2f),  10.f, 2));
        m_lights.push_back(new Lightning(glm::vec3 (0.0f, 3.f, -3.5f), glm::vec3 (0.1f, 0.f, 0.8f),  10.f, 3));
        m_lights.push_back(new Lightning(glm::vec3 (-4.5f, 3.f, -3.5f), glm::vec3 (0.6f, 0.f, 0.2f), 10.f, 4));
    }


public:
    // Constructor
    Club() {
        textures_init();
        material_init();
        meshes_init();
        models_init();
        lights_init();
    }

    // Destructor
    ~Club() {

        delete m_floor_tex;
        delete m_wall_tex;
        delete m_floor_mat;
        delete m_wall_mat;

        for (auto& mesh : m_floor_mesh) {
            delete mesh;
        }
        for (auto& mesh : m_wall_mesh) {
            delete mesh;
        }
        for (auto& mesh : m_models) {
            delete mesh;
        }
        for (auto light : m_lights) {
            delete light;
        }
    };

    // Changes light color
    void change_light_color(char type) {

        switch (type) {
            case '0':
                for (auto &i : m_lights) {
                    i->set_color(glm::vec3(0.5f));
                }
                break;
            case '1':
                m_lights[0]->set_color(glm::vec3(0.2f, 0.f, 0.5f));
                m_lights[1]->set_color(glm::vec3(0.2f, 0.f, 0.5f));
                m_lights[2]->set_color(glm::vec3(0.6f, 0.f, 0.2f));
                m_lights[3]->set_color(glm::vec3(0.1f, 0.f, 0.8f));
                m_lights[4]->set_color(glm::vec3(0.6f, 0.f, 0.2f));
                break;
            case '2':
                m_lights[0]->set_color(glm::vec3(0.8f, 0.f, 0.8f));
                m_lights[1]->set_color(glm::vec3(0.f, 0.5f, 0.8f));
                m_lights[2]->set_color(glm::vec3(0.1f, 0.f, 0.8f));
                m_lights[3]->set_color(glm::vec3(0.5f, 0.f, 0.8f));
                m_lights[4]->set_color(glm::vec3(0.8f, 0.f, 0.5f));
                break;
            case '3':
                m_lights[0]->set_color(glm::vec3(.0f, 0.5f, 0.5f));
                m_lights[1]->set_color(glm::vec3(.0f, 0.5f, 0.5f));
                m_lights[2]->set_color(glm::vec3(.0f, 0.1f, 0.6f));
                m_lights[3]->set_color(glm::vec3(.0f, 0.1f, 0.6f));
                m_lights[4]->set_color(glm::vec3(.0f, 0.1f, 0.6f));
                break;
            default:
                break;
        }
    }

    // Drops confetti
    void drop_confetti() {
        m_confetti.push_back(new Confetti());
    }

    // Update club interior
    void update(Shader* shader) {

        for (auto& i : m_lights) {
            i->send_to_shader(*shader, i->get_id());
        }
    }

    // Render club interior
    void render(Shader* shader, float delta_time) {

        for (auto& i : m_models) {
            i->render_model(shader);
        }

        for (auto i : m_confetti) {
            if (i->update(delta_time)) {
                i->render(shader);
            }
        }

    }
};


