
class Scene2 {
private:

    // Vector containing all of the lights, same as in scene 1
    std::vector<Lightning*> m_lights;

    // Background screen
    Model* m_background{};

    Bottle* m_bottle_1;
    Bottle* m_bottle_2;
    Bottle* m_bottle_3;

    // Create background screen
    void background_init() {

        auto* material = new Material(glm::vec3(0.35f), glm::vec3(1.f), glm::vec3(0.0f), 0);
        auto* texture = new Texture(R"(images\background_01.png)", GL_TEXTURE_2D, true);

        std::vector<Mesh*> background_screen;
        background_screen.push_back(new Mesh (Square().get_vertices(),  Square().get_vertices_num(),
                                       Square().get_indices(), Square().get_indices_num()));
        background_screen[0]->scale_mesh(glm::vec3(16.f, 0.f, 9.f));
        background_screen[0]->rotate_mesh(glm::vec3(90.f, 0.f, -90.f));
        background_screen[0]->set_position(glm::vec3(0.f, -3.49f, 1.5f));

        m_background = new Model(material, texture, background_screen, glm::vec3(0.f, 0.f, 0.f));
    }

    // Create lightning
    void lights_init() {
        m_lights.push_back(new Lightning(glm::vec3 (4.5f, 3.f, 5.5f),  glm::vec3 (0.2f, 0.f, 0.5f), 10.f, 0));
        m_lights.push_back(new Lightning(glm::vec3 (-4.5f, 3.f, 5.5f), glm::vec3 (0.2f, 0.f, 0.5f), 10.f, 1));

        // Backwall lights
        m_lights.push_back(new Lightning(glm::vec3 (4.5f, 3.f, -3.5f), glm::vec3 (0.6f, 0.f, 0.2f),  10.f, 2));
        m_lights.push_back(new Lightning(glm::vec3 (0.0f, 3.f, -3.5f), glm::vec3 (0.1f, 0.f, 0.8f),  10.f, 3));
        m_lights.push_back(new Lightning(glm::vec3 (-4.5f, 3.f, -3.5f), glm::vec3 (0.6f, 0.f, 0.2f), 10.f, 4));

        for (auto &i : m_lights) {
            i->set_color(glm::vec3(0.75f));
        }
    }

public:
    // Constructor
    Scene2() {

        lights_init();
        background_init();

        m_bottle_1 = new Bottle(glm::vec3(0.f, -1.15f, 0.f), 1);
        m_bottle_2 = new Bottle(glm::vec3(0.f, -0.65f, 0.f), 1);
        m_bottle_3 = new Bottle(glm::vec3(0.f,  0.15f, 0.f), 1);
    }

    // Destructor
    ~Scene2() = default;


    void update(Shader* shader) {

        for (auto& i : m_lights) {
            i->send_to_shader(*shader, i->get_id());
        }
        auto t = static_cast<float>(glfwGetTime());

        m_bottle_2->rotate_bottle(glm::vec3(t * 0.01, 0, t * 0.01), m_bottle_1->get_position());
    }

    void render(Shader* shader) {

        m_background->render_model(shader);
        m_bottle_1->render_bottle(shader);
        m_bottle_2->render_bottle(shader);
        m_bottle_3->render_bottle(shader);
    }
};