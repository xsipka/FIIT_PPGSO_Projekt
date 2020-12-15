
class Scene2 {
private:
    std::vector<Lightning*> m_lights;
    Bottle* m_bottle;

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
        m_bottle = new Bottle(glm::vec3(0.f));
    }

    // Destructor
    ~Scene2() = default;


    void update(Shader* shader) {

        for (auto& i : m_lights) {
            i->send_to_shader(*shader, i->get_id());
        }
    }

    void render(Shader* shader) {
        m_bottle->render_bottle(shader);
    }
};