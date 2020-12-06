
bool spawn_bottle = true;

class Player {
private:


public:
    // Constructor
    Player() = default;

    // Destructor
    ~Player() = default;


    static void move_player(GLFWwindow *window, Camera *camera, float delta_time,
                     double mouse_x_offset, double mouse_y_offset) {

        // Move forward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera->update_user_input(delta_time, 'F', mouse_x_offset, mouse_y_offset);
        }
        // Move backward
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera->update_user_input(delta_time, 'B', mouse_x_offset, mouse_y_offset);
        }
        // Move left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera->update_user_input(delta_time, 'L', mouse_x_offset, mouse_y_offset);
        }
        // Move right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera->update_user_input(delta_time, 'R', mouse_x_offset, mouse_y_offset);
        }
    }

    static bool check_if_near_bar(Camera *camera) {

        auto player_position = camera->get_position();

        if ((player_position.x > 1.75f && player_position.x < 5.3) &&
            (player_position.z > 3.39f && player_position.z < 3.6)) {
        return true;
        }
        return false;
    }

    static void change_camera(GLFWwindow *window, Camera *camera) {

        if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
            camera->set_cam_mode(INTERACTIVE);
        }
        if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
            camera->set_cam_mode(ANIMATED_01);
        }
        if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
            camera->set_cam_mode(ANIMATED_02);
        }
    }

    static void exit_window(GLFWwindow *window) {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }


    static void club_interaction(GLFWwindow *window, Club& club_scene, Camera *camera) {

        // Spawn a bottle
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            if (spawn_bottle) {
                spawn_bottle = false;
                if (camera->get_cam_mode() == INTERACTIVE && check_if_near_bar(camera)) {
                    club_scene.order_bottle();
                }
            }
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
            spawn_bottle = true;
        }

        // Drop confetti
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            club_scene.drop_confetti();
        }

        // Change color of light
        if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS) {
            club_scene.change_light_color('0');
        }
        if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
            club_scene.change_light_color('1');
        }
        if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
            club_scene.change_light_color('2');
        }
        if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
            club_scene.change_light_color('3');
        }
    }

    static bool delete_club_scene(GLFWwindow *window, Club& club_scene) {

        /*if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            club_scene.~Club();
            return false;
        }*/
        return true;
    }


    static void player_interaction(GLFWwindow *window, Camera *camera, Club& club_scene,
                                   float delta_time, double mouse_x_offset, double mouse_y_offset) {

        change_camera(window, camera);
        club_interaction(window, club_scene, camera);
        exit_window(window);

        if (camera->get_cam_mode() == INTERACTIVE) {
            move_player(window, camera, delta_time, mouse_x_offset, mouse_y_offset);
        }
    }

};