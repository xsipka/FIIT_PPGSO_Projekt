#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


const char INTERACTIVE = '0';
const char ANIMATED_01 = '1';
const char ANIMATED_02 = '2';


class Camera {
private:
    char m_cam_mode;

    glm::mat4 m_view_matrix;

    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_world_up;

    // animated camera positions
    glm::vec3 m_animated_01_pos;
    glm::vec3 m_animated_02_pos;

    // player camera position
    glm::vec3 m_position;

    GLfloat m_pitch{};
    GLfloat m_yaw{};

    GLfloat m_movement_speed{};
    GLfloat m_sensitivity{};


    void update_cam_vectors() {
        m_front.x = cos(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
        m_front.y = sin(glm::radians(m_pitch));
        m_front.z = sin(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));

        m_front = glm::normalize(m_front);
        m_right = glm::normalize(glm::cross(m_front, m_world_up));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

    bool check_for_collisions(float& delta_time, char direction) {

        auto shift = calculate_shift(delta_time, direction);

        // wall collisions
        if ((shift.x < 5.25f && shift.x > -5.25f) && ((shift.z < 5.25f && shift.z > -5.25f))) {

            // bar collisions
            if ((shift.x <= 5.5 && shift.x >= 1.4) &&
                (shift.z <= 4.4 && shift.z >= 3.6)) {
                return false;
            }
            if ((shift.x <= 2.1 && shift.x >= 1.4) &&
                (shift.z <= 4.8 && shift.z >= 4.2)) {
                return false;
            }
            // DJ setup & speakers collision
            if ((shift.x <= 2.35 && shift.x >= -2.2) &&
                (shift.z <= -3.5 && shift.z >= -4.8)) {
                return false;
            }
            return true;
        }
        else {
            return false;
        }
    }

    glm::vec3 calculate_shift(float& delta_time, char direction) {

        glm::vec3 shift(0.f);

        switch (direction) {
            case 'F':
                shift = m_position + m_front * m_movement_speed * delta_time;
                break;
            case 'B':
                shift = m_position - m_front * m_movement_speed * delta_time;
                break;
            case 'L':
                shift = m_position - m_right * m_movement_speed * delta_time;
                break;
            case 'R':
                shift = m_position + m_right * m_movement_speed * delta_time;
                break;
            default:
                break;
        }
        return shift;
    }

    glm::vec3 calculate_rotation() const {
        auto t = static_cast<float>(glfwGetTime());
        auto rotation = glm::vec3(0.f);

        if (m_cam_mode == ANIMATED_01) {
            rotation.x = static_cast<float>(std::sin(t) + 4.25f);
            rotation.y = 1.75f;
            rotation.z = static_cast<float>(std::cos(t) + 4.25f);
        }
        if (m_cam_mode == ANIMATED_02) {
            rotation.x = static_cast<float>(std::sin(t) + 0.25f);
            rotation.y = 1.75f;
            rotation.z = - 5.45f;
        }
        return rotation;
    }

    void update_mouse_input(float &delta_time, double &mouse_x_offset, double &mouse_y_offset) {

        // Pitch - up/down movement, Yaw - left/right movement
        m_pitch += static_cast<GLfloat>(mouse_y_offset) * m_sensitivity * delta_time;
        m_yaw += static_cast<GLfloat>(mouse_x_offset) * m_sensitivity * delta_time;

        // Locks looking up and down
        if (m_pitch > 0.f) {
            m_pitch = 0.f;
        }
        if (m_pitch < 0.f) {
            m_pitch = 0.f;
        }
        // Nullify yaw, when above 360 degrees or below -360 degrees
        if (m_yaw > 3600.f || m_yaw < -360.f) {
            m_yaw = 0;
        }
    }

    void update_key_input(float &delta_time, char direction) {
        switch (direction) {
            // Move forward
            case 'F':
                if (check_for_collisions(delta_time, direction)) {
                    m_position += m_front * m_movement_speed * delta_time;
                }
                break;
                // Move backward
            case 'B':
                if (check_for_collisions(delta_time, direction)) {
                    m_position -= m_front * m_movement_speed * delta_time;
                }
                break;
                // Move left
            case 'L':
                if (check_for_collisions(delta_time, direction)) {
                    m_position -= m_right * m_movement_speed * delta_time;
                }
                break;
                // Move right
            case 'R':
                if (check_for_collisions(delta_time, direction)) {
                    m_position += m_right * m_movement_speed * delta_time;
                }
                break;
            default:
                break;
        }
        //std::cout << "X: " << m_position.x << "\t Z: " << m_position.z << "\n";
    }

public:
    // Constructor
    Camera(glm::vec3 position, glm::vec3 world_up)  {

        // Camera settings
        m_movement_speed = 5.f;
        m_sensitivity = 10.f;
        m_cam_mode = INTERACTIVE;

        m_view_matrix = glm::mat4(1.f);
        m_world_up = world_up;
        m_up = world_up;
        m_right = glm::vec3(0.f);

        m_position = position;
        //m_animated_01_pos = glm::vec3(5.0f, 1.5f, 5.0f);
        //m_animated_02_pos = glm::vec3(0.f);

        m_pitch = 0.f;
        m_yaw = -90.f;

        update_cam_vectors();
    }

    // Destructor
    ~Camera() = default;

    // Getters & setters
    glm::vec3 get_position() {
        if (m_cam_mode == INTERACTIVE) {
            return m_position;
        }
        if (m_cam_mode == ANIMATED_01) {
            return m_animated_01_pos;
        }
        if (m_cam_mode == ANIMATED_02) {
            return m_animated_02_pos;
        }
    }

    glm::mat4 get_view_matrix() {

        if (m_cam_mode == INTERACTIVE) {
            update_cam_vectors();
            m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
        }
        if (m_cam_mode == ANIMATED_01) {
            m_animated_01_pos = calculate_rotation();
            m_view_matrix = glm::lookAt(m_animated_01_pos, glm::vec3(0.f), m_up);
        }
        if (m_cam_mode == ANIMATED_02) {
            m_animated_02_pos = calculate_rotation();
            m_view_matrix = glm::lookAt(m_animated_02_pos, glm::vec3(0.f), m_up);
        }
        return m_view_matrix;
    }

    char get_cam_mode() const {
        return m_cam_mode;
    }

    void set_cam_mode(char cam_mode) {
        m_cam_mode = cam_mode;
    }

    // Update function
    void update_user_input(float &delta_time, char direction, double &mouse_x_offset, double &mouse_y_offset) {
        update_mouse_input(delta_time, mouse_x_offset, mouse_y_offset);
        update_key_input(delta_time, direction);
    }
};