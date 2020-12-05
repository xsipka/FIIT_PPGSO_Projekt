#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include <ctime>


class Confetti {
private:
    std::vector<Texture*> m_texture{};
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
    const float BAR = 0.275f;


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
        speed.x = get_random(-20, 20);
        speed.y = -GRAVITY;
        speed.z = get_random(-20, 20);

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

        m_texture.push_back(new Texture(R"(images\confetti_01.png)", GL_TEXTURE_2D));
        m_texture.push_back(new Texture(R"(images\confetti_02.png)", GL_TEXTURE_2D));
        m_shape.push_back(new Mesh(Cube().get_vertices(),  Cube().get_vertices_num(),
                                   Cube().get_indices(), Cube().get_indices_num()));

        m_material = new Material(glm::vec3(0.5f), glm::vec3(1.f), glm::vec3(10.f), 0);
        int rand_tex = rand() % (1 - 0 + 1) + 0;
        m_model = new Model(m_material, m_texture[rand_tex], m_shape, m_position);
        m_model->scale_model(glm::vec3(CONF_LENGTH, CONF_WIDTH, CONF_LENGTH));
    }

    void wall_collisions(float delta_time) {

        auto shift = m_speed * delta_time * 0.25f;
        m_position.y += shift.y;

        if ((m_position.x + shift.x < 5.4f) && (m_position.x + shift.x > -5.4)) {
            m_position.x += shift.x;
        }
        if ((m_position.z + shift.z < 5.4f) && (m_position.z + shift.z > -5.4)) {
            m_position.z += shift.z;
        }
    }

    bool bar_collision() const {

        if ((m_position.x <= 5.5 && m_position.x >= 1.5) &&
            (m_position.z <= 4.2 && m_position.z >= 3.7)) {
            return true;
        }
        return false;
    }

public:
    // Constructor
    Confetti() {
        confetti_init();
    }

    // Destructor
    ~Confetti() {

        for (auto& i : m_texture) {
            delete i;
        }

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

        // Confetti staying on the bar
        if (m_position.y <= BAR && bar_collision()) {
            //m_position.y = BAR;
            return true;
        }

        wall_collisions(delta_time);

        // Confetti staying on the ground
        if (m_position.y <= FLOOR) { return true; }

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
    std::vector<Model*> m_bottle;

    std::vector<Texture*> m_textures;
    std::vector<Material*> m_materials;

    // Room meshes
    std::vector<Mesh*> m_floor_mesh;
    std::vector<Mesh*> m_wall_mesh;
    
    // Interior meshes (furniture & stuff)
    std::vector<std::vector<Mesh*>> m_speakers;
    std::vector<std::vector<Mesh*>> m_bar;
    std::vector<std::vector<Mesh*>> m_sitting;

    // Other meshes
    std::vector<Mesh*> m_wall_art;
    std::vector<Mesh*> m_bottle_mesh;
    std::vector<Confetti*> m_confetti;

    std::vector<Lightning*> m_lights;


    static float get_random(float min, float max) {

        static std::random_device rd;
        static std::mt19937 e2(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(e2);
        /*auto num = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
        return num;*/
    }

    // Load textures
    void textures_init() {
        m_textures.push_back(new Texture(R"(images\floor_01.png)", GL_TEXTURE_2D));         // 0
        m_textures.push_back(new Texture(R"(images\wall_01.jpg)", GL_TEXTURE_2D));          // 1
        m_textures.push_back(new Texture(R"(images\gray_tex.png)", GL_TEXTURE_2D));         // 2
        m_textures.push_back(new Texture(R"(images\wood_02.jpg)", GL_TEXTURE_2D));          // 3
        m_textures.push_back(new Texture(R"(images\wood_03.jpg)", GL_TEXTURE_2D));          // 4
        m_textures.push_back(new Texture(R"(images\basalt_01.jpg)", GL_TEXTURE_2D));        // 5
        m_textures.push_back(new Texture(R"(images\table_01.jpeg)", GL_TEXTURE_2D));        // 6
        m_textures.push_back(new Texture(R"(images\green_tex.png)", GL_TEXTURE_2D));        // 7
        m_textures.push_back(new Texture(R"(images\blue_tex.jpg)", GL_TEXTURE_2D));         // 8
        m_textures.push_back(new Texture(R"(images\magenta_tex.png)", GL_TEXTURE_2D));      // 9
        m_textures.push_back(new Texture(R"(images\dark_gray_tex.png)", GL_TEXTURE_2D));    // 10
        m_textures.push_back(new Texture(R"(images\light_gray_tex.png)", GL_TEXTURE_2D));   // 11
        m_textures.push_back(new Texture(R"(images\wallpaper.jpg)", GL_TEXTURE_2D));        // 12
    }

    // Create materials
    void material_init() {
        // walls, ceiling & floor materials
        m_materials.push_back(new Material(glm::vec3(0.25f), glm::vec3(1.f), glm::vec3(3.5f), 0));
        m_materials.push_back(new Material(glm::vec3(0.35f), glm::vec3(1.f), glm::vec3(0.0f), 0));

        // bar material
        m_materials.push_back(new Material(glm::vec3(1.25f), glm::vec3(1.f), glm::vec3(15.f), 0));

        // sitting area material
        m_materials.push_back(new Material(glm::vec3(0.25f), glm::vec3(1.f), glm::vec3(0.5f), 0));
        m_materials.push_back(new Material(glm::vec3(0.20f), glm::vec3(1.f), glm::vec3(0.5f), 0));

        // bottle material
        m_materials.push_back(new Material(glm::vec3(0.75f), glm::vec3(1.f), glm::vec3(25.f), 0));

        // speakers & DJ setup materials
        m_materials.push_back(new Material(glm::vec3(0.35f), glm::vec3(1.f), glm::vec3(15.f), 0));
        m_materials.push_back(new Material(glm::vec3(0.25f), glm::vec3(1.f), glm::vec3(5.f), 0));
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

        m_wall_art.push_back(new Mesh (Square().get_vertices(),  Square().get_vertices_num(),
                                       Square().get_indices(), Square().get_indices_num()));
        m_wall_art[0]->scale_mesh(glm::vec3(4.75f, 0.f, 3.25f));
        m_wall_art[0]->rotate_mesh(glm::vec3(-90.f, 0.f, -90.f));
        m_wall_art[0]->set_position(glm::vec3(0.f, -5.49f, 1.5f));
    }

    // Loads .obj files into meshes
    void load_obj_files() {

        m_bar.resize(4);
        m_speakers.resize(5);
        m_sitting.resize(3);
        std::vector<Vertex> temp;

        // Bar & stools
        temp = load_obj_file("obj_files/bar_01.obj");
        m_bar[0].push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));
        temp = load_obj_file("obj_files/bar_02.obj");
        m_bar[1].push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));
        temp = load_obj_file("obj_files/bar_03.obj");
        m_bar[2].push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));
        temp = load_obj_file("obj_files/bar_04.obj");
        m_bar[3].push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));

        // Speakers & DJ setup
        temp = load_obj_file("obj_files/speaker_01.obj");
        m_speakers[0].push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));
        temp = load_obj_file("obj_files/speaker_02.obj");
        m_speakers[1].push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));
        temp = load_obj_file("obj_files/speaker_03.obj");
        m_speakers[2].push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));
        temp = load_obj_file("obj_files/speaker_04.obj");
        m_speakers[3].push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));
        temp = load_obj_file("obj_files/speaker_05.obj");
        m_speakers[4].push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));

        // Sitting area
        temp = load_obj_file("obj_files/sitting_01.obj");
        m_sitting[0].push_back(new Mesh(temp.data(), temp.size(), nullptr, 0));
        temp = load_obj_file("obj_files/sitting_02.obj");
        m_sitting[1].push_back(new Mesh(temp.data(), temp.size(), nullptr, 0));
        temp = load_obj_file("obj_files/sitting_03.obj");
        m_sitting[2].push_back(new Mesh(temp.data(), temp.size(), nullptr, 0));

        // Wine bottle
        temp = load_obj_file("obj_files/wine_01.obj");
        m_bottle_mesh.push_back(new Mesh(temp.data(), temp.size(), nullptr, 0));
    }

    // Put everything together and create whole club
    void models_init() {
        // walls, ceiling & floor
        m_models.push_back(new Model(m_materials[0], m_textures[0], m_floor_mesh, glm::vec3(0.f, -0.75f, 0.f)));
        m_models.push_back(new Model(m_materials[1], m_textures[1], m_floor_mesh, glm::vec3(0.f, 3.75f, 0.f)));
        m_models.push_back(new Model(m_materials[1], m_textures[1], m_wall_mesh, glm::vec3(0.f, 0.f, 0.f)));

        m_models.push_back(new Model(m_materials[1], m_textures[12], m_wall_art, glm::vec3(0.f, 0.f, 0.f)));

        // bar
        m_models.push_back(new Model(m_materials[0], m_textures[3], m_bar[0], glm::vec3(1.75f, -0.75f, 4.25f)));
        m_models.push_back(new Model(m_materials[0], m_textures[4], m_bar[1], glm::vec3(1.75f, -0.75f, 4.25f)));
        m_models.push_back(new Model(m_materials[2], m_textures[5], m_bar[2], glm::vec3(1.75f, -0.75f, 4.25f)));
        m_models.push_back(new Model(m_materials[0], m_textures[4], m_bar[3], glm::vec3(1.75f, -0.75f, 4.25f)));

        // wine bottle      // X: ( -1.5, 1.75)  Z: ( 3.9, 4.3)
        m_bottle.push_back(new Model(m_materials[5], m_textures[7], m_bottle_mesh, glm::vec3(1.5f, -0.75f, 4.3f)));
        //std::cout << get_random(-1.5, 1.75);

        // speakers & DJ setup
        m_models.push_back(new Model(m_materials[7], m_textures[2], m_speakers[0], glm::vec3(-2.f, -0.75f, -3.75f)));
        m_models.push_back(new Model(m_materials[6], m_textures[11], m_speakers[1], glm::vec3(-2.f, -0.75f, -3.75f)));
        m_models.push_back(new Model(m_materials[7], m_textures[10], m_speakers[2], glm::vec3(-2.f, -0.75f, -3.75f)));
        m_models.push_back(new Model(m_materials[5], m_textures[8], m_speakers[3], glm::vec3(-2.f, -0.75f, -3.75f)));
        m_models.push_back(new Model(m_materials[7], m_textures[9], m_speakers[4], glm::vec3(-2.f, -0.75f, -3.75f)));

        // sitting area
        m_models.push_back(new Model(m_materials[3], m_textures[6], m_sitting[0], glm::vec3(-5.1f, -.75f, 5.1f)));
        m_models.push_back(new Model(m_materials[3], m_textures[9], m_sitting[1], glm::vec3(-5.1f, -.75f, 5.1f)));
        m_models.push_back(new Model(m_materials[4], m_textures[2], m_sitting[2], glm::vec3(-5.1f, -.75f, 5.1f)));
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
        load_obj_files();
        models_init();
        lights_init();
    }

    // Destructor
    ~Club() {

        for (auto& i : m_textures) {
            delete i;
        }
        for (auto& i : m_materials) {
            delete i;
        }
        for (auto& mesh : m_floor_mesh) {
            delete mesh;
        }
        for (auto& mesh : m_wall_mesh) {
            delete mesh;
        }
        for (auto& model : m_models) {
            delete model;
        }
        for (auto& bottle : m_bottle) {
            delete bottle;
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
                    i->set_color(glm::vec3(0.75f));
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
                m_lights[0]->set_color(glm::vec3(.4f, 0.5f, 0.6f));
                m_lights[1]->set_color(glm::vec3(.4f, 0.5f, 0.6f));
                m_lights[2]->set_color(glm::vec3(.1f, 0.1f, 0.6f));
                m_lights[3]->set_color(glm::vec3(.4f, 0.1f, 0.6f));
                m_lights[4]->set_color(glm::vec3(.1f, 0.1f, 0.6f));
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

        for (auto& i : m_bottle) {
            i->render_model(shader);
        }

        for (auto& i : m_confetti) {
            if (i->update(delta_time)) {
                i->render(shader);
            }
        }

    }
};


