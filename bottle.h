
class Bottle {
private:
    Model* m_model;
    Texture* m_texture;
    Material* m_material;
    std::vector<Mesh*> m_mesh;

    float m_time_to_live = 10.f;
    glm::vec3 m_speed;
    glm::vec3 m_position;

public:
    // Constructor
    explicit Bottle(glm::vec3 position) {

        m_position = position;

        std::vector<Vertex> temp;
        temp = load_obj_file("obj_files/wine_02.obj");
        m_mesh.push_back(new Mesh(temp.data(), temp.size(), nullptr, 0));

        m_texture = new Texture(R"(images\green_tex.png)", GL_TEXTURE_2D);
        m_material = new Material(glm::vec3(0.75f), glm::vec3(1.f), glm::vec3(25.f), 0);
        m_model = new Model(m_material, m_texture, m_mesh, m_position);
    }

    // Destructor
    ~Bottle() = default;


    glm::vec3 get_position() {
        return m_model->get_position();
    }

    void set_position(glm::vec3 position) {
        m_model->set_position(position);
    }

    glm::vec3 get_speed() {
        return m_speed;
    }

    void set_speed(glm::vec3 speed) {
        m_speed = speed;
    }

    float get_time_to_live() const {
        return m_time_to_live;
    }

    void increase_speed(glm::vec3 boost) {
        m_speed += boost;
    }

    void decrease_time_to_live(float num) {
        m_time_to_live -= num;
    }

    void move_bottle(glm::vec3 shift) {
        m_model->move_model(shift);
    }

    void render_bottle(Shader* shader) {
        m_model->render_model(shader);
    }
};