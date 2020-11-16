
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


    static void exit_window(GLFWwindow *window) {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }


    static bool delete_club_scene(GLFWwindow *window, Club& club_scene) {

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            club_scene.~Club();
            return false;
        }
        return true;
    }


    static void player_interaction(GLFWwindow *window, Camera *camera, float delta_time,
                            double mouse_x_offset, double mouse_y_offset) {

        move_player(window, camera, delta_time, mouse_x_offset, mouse_y_offset);
        exit_window(window);
    }

};