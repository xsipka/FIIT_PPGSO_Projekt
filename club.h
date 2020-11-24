
class Club {
private:
    std::vector<Model*> m_models;

    Texture* m_wall_tex;
    Texture* m_floor_tex;

    Material* m_wall_mat;
    Material* m_floor_mat;

    std::vector<Mesh*> m_floor_mesh;
    std::vector<Mesh*> m_wall_mesh;

    std::vector<Lightning*> m_lights;

public:
    // Constructor
    Club() {
        // Load textures
        m_floor_tex = new Texture(R"(images\floor_01.png)", GL_TEXTURE_2D);
        m_wall_tex = new Texture(R"(images\wall_01.jpg)", GL_TEXTURE_2D);

        // Create materials
        m_floor_mat = new Material(glm::vec3(.15f), glm::vec3(1.f), glm::vec3(3.5f), 0);
        m_wall_mat = new Material(glm::vec3(0.35f), glm::vec3(1.f), glm::vec3(0.1f), 0);

        // Floor & ceiling meshes
        m_floor_mesh.push_back(new Mesh (Ground().get_vertices(),  Ground().get_vertices_num(),
                                      Ground().get_indices(), Ground().get_indices_num()));
        m_floor_mesh[0]->scale_mesh(glm::vec3(10.f));

        // Wall meshes
        m_wall_mesh.push_back(new Mesh (Walls().get_vertices(),  Walls().get_vertices_num(),
                                      Walls().get_indices(), Walls().get_indices_num()));
        m_wall_mesh[0]->scale_mesh(glm::vec3(10.f));

        /*std::vector<Vertex> temp;
        temp = load_obj_file("obj_files/kock.obj");
        m_meshes.push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));*/

        // Floor, ceiling & walls
        m_models.push_back(new Model(m_floor_mat, m_floor_tex, m_floor_mesh, glm::vec3(0.f, -0.75f, 0.f)));
        m_models.push_back(new Model(m_floor_mat, m_wall_tex, m_floor_mesh, glm::vec3(0.f, 3.75f, 0.f)));
        m_models.push_back(new Model(m_wall_mat, m_wall_tex, m_wall_mesh, glm::vec3(0.f, 0.f, 0.f)));

        m_lights.push_back(new Lightning(glm::vec3 (4.5f, 3.f, 5.5f), glm::vec3 (0.8f, 0.f, 0.8f), 10.f, 0));
        m_lights.push_back(new Lightning(glm::vec3 (-4.5f, 3.f, 5.5f), glm::vec3 (0.f, 0.8f, 0.8f), 10.f, 1));
        m_lights.push_back(new Lightning(glm::vec3 (0.f, 3.f, -5.25f), glm::vec3 (0.f, 0.8f, 0.f), 10.f, 2));
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


    // Render club interior
    void render(Shader* shader) {

        for (auto& i : m_lights) {
            i->send_to_shader(*shader, i->get_id());
        }

        for (auto& i : m_models) {
            i->render_model(shader);
        }
    }
};