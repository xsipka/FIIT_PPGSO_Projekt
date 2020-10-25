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
    glm::vec3 m_up_view;
    glm::vec3 m_front_view;
    glm::vec3 m_camera_pos;
    glm::mat4 m_view_matrix;

    // Projection matrix
    float m_fov{};
    float m_near_plane{};
    float m_far_plane{};
    glm::mat4 m_projection_matrix;

    std::vector<PointLight*> m_lights;
    std::vector<Shader*> m_shaders;
    std::vector<Texture*> m_textures;
    std::vector<Mesh*> m_meshes;
    std::vector<Material*> m_materials;
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
        m_view_matrix = glm::lookAt(m_camera_pos, m_camera_pos + m_front_view, m_up_view);

        m_projection_matrix = glm::mat4(1.f);
        m_projection_matrix = glm::perspective(
                glm::radians(m_fov),
                static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
                m_near_plane, m_far_plane);
    }

    void lights_init() {
        m_lights.push_back(new PointLight(glm::vec3 (0.f, 0.f, 1.f), glm::vec3 (0.f, 1.f, 1.f), 10.f));
    }

    void shaders_init() {
        m_shaders.push_back(new Shader("vertex.glsl", "fragment.glsl", ""));
    }

    void textures_init() {
        m_textures.push_back(new Texture(R"(images\linus_sex_tips.png)", GL_TEXTURE_2D));
        m_textures.push_back(new Texture(R"(images\xd.png)", GL_TEXTURE_2D));
    }

    void meshes_init() {
        /*m_meshes.push_back(new Mesh (Pyramid().get_vertices(),  Pyramid().get_vertices_num(),
                                         Pyramid().get_indices(), Pyramid().get_indices_num()));*/

        m_meshes.push_back(new Mesh (Ground().get_vertices(),  Ground().get_vertices_num(),
                                     Ground().get_indices(), Ground().get_indices_num()));
        m_meshes[0]->scale_mesh(glm::vec3(25.f));

        /*std::vector<Vertex> temp;
        temp = load_obj_file("obj_files/kock.obj");

        m_meshes.push_back(new Mesh (temp.data(), temp.size(), nullptr, 0));*/
    }

    void materials_init() {
        m_materials.push_back(new Material(glm::vec3(.25f), glm::vec3(.5f), glm::vec3(1.f), 0, 1));
    }

    void models_init() {
        m_models.push_back(new Model(m_materials[0], m_textures[0], m_textures[0], m_meshes, glm::vec3(0.f, -0.2f, 0.f)));
        //m_models.push_back(new Model(m_materials[0], m_textures[0], m_textures[0], m_meshes, glm::vec3(0.f, -0.3f, 2.f)));
        //m_models.push_back(new Model(m_materials[0], m_textures[0], m_textures[0], m_meshes, glm::vec3(2.f, -0.3f, 0.f)));

        //m_models.push_back(new Model(m_materials[0], m_textures[0], m_textures[0], m_meshes, glm::vec3(0.f, 1.f, 1.f)));
        //m_models.push_back(new Model(m_materials[0], m_textures[0], m_textures[0], m_meshes, glm::vec3(0.f, 0.3f, 2.f)));
        //m_models.push_back(new Model(m_materials[0], m_textures[0], m_textures[0], m_meshes, glm::vec3(2.f, 0.3f, 0.f)));
    }

    void uniforms_init() {
        m_shaders[0]->set_gl_mat4(m_view_matrix, "view_matrix", GL_FALSE);
        m_shaders[0]->set_gl_mat4(m_projection_matrix, "projection_matrix", GL_FALSE);
        m_lights[0]->send_to_shader(*m_shaders[0]);
    }

    // Update functions
    void uniforms_update() {

        //this->shaders[0]->use_program();
        m_view_matrix = m_camera.get_view_matrix();
        /*m_projection_matrix = glm::perspective(
                glm::radians(m_fov),
                static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
                m_near_plane, m_far_plane);*/
        m_shaders[0]->set_gl_mat4(m_projection_matrix, "projection_matrix", GL_FALSE);
        m_shaders[0]->set_gl_mat4(m_view_matrix, "view_matrix", GL_FALSE);
        m_shaders[0]->set_gl_vec3(m_camera.get_position(), "camera_pos");
        //*m_lights[0] = m_camera.get_position();
        m_lights[0]->send_to_shader(*m_shaders[0]);
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

        if (glfwGetKey(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
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

        m_up_view = glm::vec3(0.f, 1.f, 0.f);
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
        shaders_init();
        textures_init();
        materials_init();
        meshes_init();
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

        //std::cout << "X: " << m_mouse_x_offset << "\tY: " << m_mouse_y_offset << "\n";

        move_player();

        /*for (auto& i : m_models) {
            i->rotate_model(glm::vec3(0, 0.1, 0));
        }*/

        exit_window();
    }

    // Main render function
    void render() {

        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Updates uniforms
        uniforms_update();
        /*m_materials[0]->send_to_shader(*m_shaders[0]);
        m_shaders[0]->use_program();

        // Activate texture
        m_textures[0]->bind_texture(GL_TEXTURE_2D, 0);
        m_textures[0]->bind_texture(GL_TEXTURE_2D, 1);

        // Draw
        m_meshes[0]->render(m_shaders[0]);*/

        // Render models
        for (auto& i : m_models) {
            i->render_model(m_shaders[0]);
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
            if (!collision_detection())
            m_camera.update_user_input(m_delta_time, 'F', m_mouse_x_offset, m_mouse_y_offset);
        }
        // Move backward
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            if (!collision_detection())
            m_camera.update_user_input(m_delta_time, 'B', m_mouse_x_offset, m_mouse_y_offset);
        }
        // Move left
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            if (!collision_detection())
            m_camera.update_user_input(m_delta_time, 'L', m_mouse_x_offset, m_mouse_y_offset);
        }
        // Move right
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            if (!collision_detection())
            m_camera.update_user_input(m_delta_time, 'R', m_mouse_x_offset, m_mouse_y_offset);
        }
    }

    bool collision_detection() {

        for (auto& i : m_models) {
            i->get_position();
            /*std::cout << "Model: x: " << i->get_position().x << ", z: " << i->get_position().z;
            std::cout << " Camera: x: " << m_camera.get_position().x << ", z: " << m_camera.get_position().z << "\n";*/
        }
        return false;
    }
};