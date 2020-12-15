#include "libraries.h"


class GameWindow {
private:
    // Window sizes
    GLFWwindow* m_window{};
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;

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

    Club* m_club_scene{};
    bool m_club_existence;
    Scene2* m_scene_2{};

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
    Player m_player;

    // Creates window
    void window_init(const char *title) {

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        int m_framebuffer_width = 0;
        int m_framebuffer_height = 0;
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

    void shader_init() {
        m_shader = new Shader("vertex.glsl", "fragment.glsl");
    }

    void uniforms_init() {
        m_shader->set_gl_mat4(m_view_matrix, "view_matrix", GL_FALSE);
        m_shader->set_gl_mat4(m_projection_matrix, "projection_matrix", GL_FALSE);
    }

    // Update functions
    void uniforms_update() {

        m_view_matrix = m_camera.get_view_matrix();
        m_shader->set_gl_mat4(m_projection_matrix, "projection_matrix", GL_FALSE);
        m_shader->set_gl_mat4(m_view_matrix, "view_matrix", GL_FALSE);
        m_shader->set_gl_vec3(m_camera.get_position(), "camera_pos");
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
    }


public:
    // Constructor
    GameWindow(const int width, const int height, const char *title, Camera camera)
            : WINDOW_WIDTH(width), WINDOW_HEIGHT(height), m_camera(camera) {

        GameWindow::glfw_init();
        window_init(title);
        GameWindow::glew_init();
        GameWindow::options_init();

        m_world_up = glm::vec3(0.f, 1.f, 0.f);
        m_front_view = glm::vec3(0.f, 0.f, -1.f);
        m_camera_pos = glm::vec3(0.f, 0.f, 1.f);

        m_fov = 90.f;
        m_near_plane = 0.1f;
        m_far_plane = 100.f;

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
        uniforms_init();

        m_club_scene = new Club();
        m_club_existence = true;
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

        if (m_camera.get_cam_mode() == INTERACTIVE) {
            m_camera.update_user_input(m_delta_time, -1, m_mouse_x_offset, m_mouse_y_offset);
        }

        glfwPollEvents();

        Player::exit_window(m_window);
        Player::update_movement(m_window, &m_camera, m_delta_time, m_mouse_x_offset, m_mouse_y_offset);


        if (m_club_existence) {
            m_club_scene->update(m_shader);
            m_player.player_interaction(m_window, &m_camera, *m_club_scene, m_delta_time);
            m_club_existence = m_player.switch_scenes(*m_club_scene);
            if (!m_club_existence) {
                m_scene_2 = new Scene2();
            }
        }
        if (!m_club_existence) {
            m_scene_2->update(m_shader);
        }
    }

    // Main render function
    void render() {

        // Clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Updates uniforms
        uniforms_update();

        if (m_club_existence) {
            m_club_scene->render(m_shader, m_delta_time);
        }
        if (!m_club_existence) {
            m_scene_2->render(m_shader);
        }

        // End draw
        glfwSwapBuffers(m_window);
        glFlush();

        glBindVertexArray(0);
        Texture::unbind_texture(GL_TEXTURE_2D);
        glUseProgram(0);
    }
};