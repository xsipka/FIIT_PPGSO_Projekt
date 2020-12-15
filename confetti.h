
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
    const float BAR = 0.27f;


    static float get_random(float min, float max) {

        auto num = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
        return num;
    }

    static glm::vec3 randomize_position() {

        auto position = glm::vec3(0.f);
        position.x = get_random(-5.4f, 5.4f);
        position.y = 3.75f;
        position.z = get_random(-5.4f, 5.4f);

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

    bool wall_collisions(float delta_time) {

        auto shift = m_speed * delta_time * 0.25f;
        m_position.y += shift.y;

        if ((m_position.x + shift.x < 5.4f) && (m_position.x + shift.x > -5.4)) {
            m_position.x += shift.x;
            if ((m_position.z + shift.z < 5.4f) && (m_position.z + shift.z > -5.4)) {
                m_position.z += shift.z;
                return false;
            }
        }
        return true;
    }

    bool bar_collision() const {

        if ((m_position.x <= 5.5 && m_position.x >= 1.5) &&
            (m_position.z <= 4.3 && m_position.z >= 3.8)) {
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

        if (wall_collisions(delta_time)) {
            m_speed.x = -m_speed.x;
            m_speed.z = -m_speed.z;
        }

        // Confetti staying on the ground
        if (m_position.y <= FLOOR) { return true; }

        // Confetti staying on the bar
        if (m_position.y <= BAR && bar_collision()) {
            //m_position.y = BAR;
            //std::cout << m_position.x << " " << m_position.y << " " << m_position.z << "\n";
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