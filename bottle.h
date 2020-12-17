#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include <ctime>

const float OFFSET = 0.1f;
const float SPEED = 0.05f;
const float RESISTANCE = 0.05f;


class Bottle {
private:
    Model* m_model;
    Texture* m_texture;
    Material* m_material;
    std::vector<Mesh*> m_bottle_mesh;

    Model* m_shard_1{};
    Model* m_shard_2{};
    Model* m_shard_3{};
    std::vector<Mesh*> m_shard_mesh;

    float m_time_to_live = 10.f;
    bool m_broken;

    glm::vec3 m_speed;
    glm::vec3 m_position;

    float m_counter = 5.f;


    static float get_random(float min, float max)  {

        static std::random_device rd;
        static std::mt19937 e2(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(e2);
    }

    glm::vec3 randomize_position() const {

        auto position = glm::vec3();
        position.x = get_random(m_position.x - OFFSET, m_position.x + OFFSET);
        position.y = m_position.y;
        position.z = get_random(m_position.z - OFFSET, m_position.z + OFFSET);

        return position;
    }

    void calculate_speed() {
        m_speed.x = SPEED;
        m_speed.y = 0.f;
        m_speed.z = SPEED;
    }

    static bool chance() {
        return (rand() % 2) != 0;
    }

    static glm::vec3 randomize_direction(glm::vec3 vec) {

        if (vec.x < 0) { return vec; }
        if (vec.z < 0) { return vec; }

        if (chance()) {
            vec.x = -vec.x;
        }
        if (chance()) {
            vec.z = -vec.z;
        }
        return vec;
    }

    static void move_shard(glm::vec3 shift, Model* shard) {
        randomize_direction(shift);
        shard->move_model(shift);
    }

    void move_shards(glm::vec3 shift) {
        move_shard(shift, m_shard_1);
        move_shard(-shift, m_shard_2);
        move_shard(glm::vec3(shift.x, shift.y, -shift.z), m_shard_3);
    }

    void shards_init() {

        std::vector<Vertex> temp;
        temp = load_obj_file("obj_files/shard_01.obj");
        m_shard_mesh.push_back(new Mesh(temp.data(), temp.size(), nullptr, 0));

        m_shard_1 = new Model(m_material, m_texture, m_shard_mesh, randomize_position());
        m_shard_2 = new Model(m_material, m_texture, m_shard_mesh, randomize_position());
        m_shard_3 = new Model(m_material, m_texture, m_shard_mesh, randomize_position());

        calculate_speed();
    }

public:
    // Constructor
    explicit Bottle(glm::vec3 position, int bottle_type = 2) {

        m_position = position;
        m_broken = false;

        std::vector<Vertex> temp;

        if (bottle_type == 1) {
            temp = load_obj_file("obj_files/wine_01.obj");
        }
        else {
            temp = load_obj_file("obj_files/wine_02.obj");
        }
        m_bottle_mesh.push_back(new Mesh(temp.data(), temp.size(), nullptr, 0));

        m_texture = new Texture(R"(images\green_tex.png)", GL_TEXTURE_2D);
        m_material = new Material(glm::vec3(0.75f), glm::vec3(1.f), glm::vec3(25.f), 0);
        m_model = new Model(m_material, m_texture, m_bottle_mesh, m_position);
    }

    // Destructor
    ~Bottle() = default;

    // Getters & setters
    glm::vec3 get_position() {
        m_position = m_model->get_position();
        return m_position;
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

    glm::vec3 get_rotation() {
        return m_model->get_rotation();
    }

    bool get_broken_status() const {
        return m_broken;
    }

    void set_broken_status(bool status) {
        m_broken = status;
        if (m_shard_mesh.empty()) {
            shards_init();
        }
    }

    float get_time_to_live() const {
        return m_time_to_live;
    }

    // Various modifiers
    void increase_speed(glm::vec3 boost) {
        m_speed += boost;
    }

    void decrease_time_to_live(float num) {
        m_time_to_live -= num;
    }

    void move_bottle(glm::vec3 shift) {
        m_model->move_model(shift);
    }

    void rotate_bottle(glm::vec3 shift, glm::vec3 position) {
        m_model->rotate_model(shift);
        m_model->rotate_around(position);
    }

    // Update position of broken pieces of glass
    void update_breaking_animation() {

        if (m_counter > 0) {
            m_counter -= RESISTANCE;
        }
        else { m_counter = 0.f; }

        // Slowly decrease speed of moving shards
        move_shards(m_speed * 0.01f * m_counter);
    }

    // Main render function
    void render_bottle(Shader* shader) {

        if (!m_broken) {
            m_model->render_model(shader);
        }
        else {
            m_shard_1->render_model(shader);
            m_shard_2->render_model(shader);
            m_shard_3->render_model(shader);
        }
    }
};