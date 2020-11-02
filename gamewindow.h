#include "libraries.h"


class GameWindow {
private:
    // Window sizes
    GLFWwindow* m_window{};
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;
    int m_framebuffer_width{};
    int m_framebuffer_height{};

    // View matrix
    glm::vec3 m_world_up;
    glm::vec3 m_front_view;
    glm::vec3 m_camera_pos;
    glm::mat4 m_view_matrix;

    // Projection matrix
    float m_fov{};
    float m_near_plane{};
    float m_far_plane{};
    glm::mat4 m_projection_matrix;

    // Shader
    Shader* m_shader{};

    // Vector containing all light sources
    std::vector<Lightning*> m_lights;

    // Vector containing all models
    std::vector<Model*> m_models;

    // Delta time & mouse input
    float m_delta_time;
    float m_curr_time;
    float m_prev_time;
    bool m_mouse_first;
    double m_mouse_x;
    double m_mouse_y;
    double m_mouse_x_last;
    double m_mouse_y_last;
    double m_mouse_x_offset;
    double m_mouse_y_offset;
    Camera m_camera;


    // Creates window
    void window_init(const char *title) {
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        this->m_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, nullptr, nullptr);

        if (!m_window) {
            std::cout << "Error in gamewindow.h, window_init(): Window couldn't be created ...\n";
            glfwTerminate();
            exit(1);
        }

        glfwGetFramebufferSize(m_window, &m_framebuffer_width, &m_framebuffer_height);
        glViewport(0, 0, m_framebuffer_width, m_framebuffer_height);

        glfwMakeContextCurrent(m_window);
    }

    // GLFW init
    static void glfw_init() {

        if (!glfwInit()) {
            std::cout << "Error in gamewindow.h, glfw_init(): GLFW was not initialized ...\n";
            exit(1);
        }
    }

    // GLEW init
    static void glew_init() {

        GLenum err = glewInit();
        if (GLEW_OK != err) {
            std::cout << "Error in gamewindow.h, glew_init(): " << glewGetErrorString(err) << "\n";
            glfwTerminate();
            exit(1);
        }
    }

    // Sets up some OpenGL things
    void options_init() {

        glEnable(GL_DEPTH_TEST);

        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        //glFrontFace(GL_CCW);

        // Blending of colors
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Use cursor
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    // Init functions
    void matrix_init() {
        m_view_matrix = glm::mat4(1.f);
        m_view_matrix = glm::lookAt(m_camera_pos, m_camera_pos + m_front_view, m_world_up);

        m_projection_matrix = glm::mat4(1.f);
        m_projection_matrix = glm::perspective(
                glm::radians(m_fov),
                static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
                m_near_plane, m_far_plane);
    }

    void lights_init() {
        m_lights.push_back(new Lightning(glm::vec3 (0.f, 1.f, 0.f), glm::vec3 (0.8f, 0.f, 0.8f), 10.f));
    }

    void shader_init() {
        m_shader = new Shader("vertex.glsl", "fragment.glsl");
    }

    // Split into to functions with parameters
    void models_init() {

        // Create textures
        auto *texture_01 = new Texture(R"(images\floor_01.png)", GL_TEXTURE_2D);
        auto *texture_02 = new Texture(R"(images\ceiling_01.jpg)", GL_TEXTURE_2D);

        // Create material
        auto *material = new Material(glm::vec3(1.25f), glm::vec3(0.5f), glm::vec3(0.5f), 0);

        // Create meshes
        std::vector<Mesh*> m_meshes;
        m_meshes.push_back(new Mesh (Ground().get_vertices(),  Ground().get_vertices_num(),
                                     Ground().get_indices(), Ground().get_indices_num()));
        m_meshes[0]->scale_mesh(glm::vec3(10.f));

        std::vector<Mesh*> wall_mesh;
        wall_mesh.push_back(new Mesh (Walls().get_vertices(),  Walls().get_vertices_num(),
                                     Walls().get_indices(), Walls().get_indices_num()));
        wall_mesh[0]->scale_mesh(glm::vec3(10.f));

        /*std::vector<Vertex> temp;
        temp = load_obj_file("obj_files/kock.obj");
        m_meshes.push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));*/

        // Walls
        //m_models.push_back(new Model(material, diffuse_tex, m_meshes, glm::vec3(0.f, 0.f, 0.f)));

        // Floor & ceiling
        m_models.push_back(new Model(material, texture_01, m_meshes, glm::vec3(0.f, -0.75f, 0.f)));
        m_models.push_back(new Model(material, texture_02, m_meshes, glm::vec3(0.f, 2.25f, 0.f)));
        m_models.push_back(new Model(material, texture_02, wall_mesh, glm::vec3(0.f, 0.f, 0.f)));
    }

    void uniforms_init() {
        m_shader->set_gl_mat4(m_view_matrix, "view_matrix", GL_FALSE);
        m_shader->set_gl_mat4(m_projection_matrix, "projection_matrix", GL_FALSE);
        m_lights[0]->send_to_shader(*m_shader);
    }

    // Update functions
    void uniforms_update() {

        m_view_matrix = m_camera.get_view_matrix();
        /*m_projection_matrix = glm::perspective(
                glm::radians(m_fov),
                static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
                m_near_plane, m_far_plane);*/
        m_shader->set_gl_mat4(m_projection_matrix, "projection_matrix", GL_FALSE);
        m_shader->set_gl_mat4(m_view_matrix, "view_matrix", GL_FALSE);
        m_shader->set_gl_vec3(m_camera.get_position(), "camera_pos");
        m_lights[0]->send_to_shader(*m_shader);
    }

    void update_delta_time() {
        m_curr_time = static_cast<float>(glfwGetTime());
        m_delta_time = m_curr_time - m_prev_time;
        m_prev_time = m_curr_time;
    }

    void update_mouse_position() {

        glfwGetCursorPos(m_window, &m_mouse_x, &m_mouse_y);

        // If mouse hadn't been moved yet
        if (m_mouse_first) {
            m_mouse_x_last = m_mouse_x;
            m_mouse_y_last = m_mouse_y;
            m_mouse_first = false;
        }
        m_mouse_x_offset = m_mouse_x - m_mouse_x_last;
        m_mouse_y_offset = m_mouse_y_last - m_mouse_y;
        m_mouse_x_last = m_mouse_x;
        m_mouse_y_last = m_mouse_y;

        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS) {
            m_lights[0]->set_position(m_camera.get_position());
        }
    }


public:
    // Constructor
    GameWindow(const int width, const int height, const char *title, int &framebuffer_width, int &framebuffer_height, Camera camera)
            : WINDOW_WIDTH(width), WINDOW_HEIGHT(height), m_camera(camera) {

        m_framebuffer_width = framebuffer_width;
        m_framebuffer_height = framebuffer_height;

        GameWindow::glfw_init();
        window_init(title);
        GameWindow::glew_init();
        GameWindow::options_init();

        m_world_up = glm::vec3(0.f, 1.f, 0.f);
        m_front_view = glm::vec3(0.f, 0.f, -1.f);
        m_camera_pos = glm::vec3(0.f, 0.f, 1.f);

        m_fov = 90.f;
        m_near_plane = 0.1f;
        m_far_plane = 1000.f;

        m_delta_time = 0;
        m_curr_time = 0;
        m_prev_time = 0;

        m_mouse_first = true;
        m_mouse_x = 0;
        m_mouse_y = 0;
        m_mouse_x_last = 0;
        m_mouse_y_last = 0;
        m_mouse_x_offset = 0;
        m_mouse_y_offset = 0;

        matrix_init();
        shader_init();
        models_init();
        lights_init();
        uniforms_init();
    }

    // Destructor
    ~GameWindow() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    // Getter
    GLFWwindow* get_window() { return m_window; }

    // Main update function
    void update() {

        update_delta_time();
        update_mouse_position();
        m_camera.update_user_input(m_delta_time, -1, m_mouse_x_offset, m_mouse_y_offset);

        glfwPollEvents();

        move_player();
        exit_window();

        /*for (auto& i : m_models) {
            i->rotate_model(glm::vec3(0, 0.1, 0));
        }*/
    }

    // Main render function
    void render() {

        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Updates uniforms
        uniforms_update();

        // Render models
        for (auto& i : m_models) {
            i->render_model(m_shader);
        }

        // End draw
        glfwSwapBuffers(m_window);
        glFlush();

        glBindVertexArray(0);
        Texture::unbind_texture(GL_TEXTURE_2D);
        glUseProgram(0);
    }

    // Closes window after pressing esc
    void exit_window() {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }
    }

    // Moves the camera
    void move_player() {
        // Move forward
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            if (!collision_detection('F')) {
                m_camera.update_user_input(m_delta_time, 'F', m_mouse_x_offset, m_mouse_y_offset);
            }
        }
        // Move backward
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            if (!collision_detection('B')) {
                m_camera.update_user_input(m_delta_time, 'B', m_mouse_x_offset, m_mouse_y_offset);
            }
        }
        // Move left
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            if (!collision_detection('L')) {
                m_camera.update_user_input(m_delta_time, 'L', m_mouse_x_offset, m_mouse_y_offset);
            }
        }
        // Move right
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            if (!collision_detection('R')) {
                m_camera.update_user_input(m_delta_time, 'R', m_mouse_x_offset, m_mouse_y_offset);
            }
        }
    }

    // Checks Player - walls collisions
    bool collision_detection(char direction) {

        std::cout << " Camera: x: " << m_camera.get_position().x << ", z: " << m_camera.get_position().z << "\n";
        return false;
    }
};