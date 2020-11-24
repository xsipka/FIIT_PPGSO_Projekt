#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class Camera {
private:
    glm::mat4 m_view_matrix;
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_up_view;

    GLfloat m_pitch{};
    GLfloat m_yaw{};

    GLfloat m_movement_speed{};
    GLfloat m_sensitivity{};


    void update_cam_vectors() {
        m_front.x = cos(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
        m_front.y = sin(glm::radians(m_pitch));
        m_front.z = sin(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));

        m_front = glm::normalize(m_front);
        m_right = glm::normalize(glm::cross(m_front, m_up_view));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

    bool camera_constraint(float& delta_time, char direction) {

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

        if ((shift.x < 5.25f && shift.x > -5.25f) && ((shift.z < 5.25f && shift.z > -5.25f))) {
            return true;
        }
        else {
            return false;
        }
    }


public:
    // Constructor
    Camera(glm::vec3 position, glm::vec3 up_view) {

        // Camera settings
        m_movement_speed = 5.f;
        m_sensitivity = 10.f;

        m_view_matrix = glm::mat4(1.f);
        m_position = position;
        m_up_view = up_view;

        m_up = up_view;
        m_right = glm::vec3(0.f);

        m_pitch = 0.f;
        m_yaw = -90.f;

        update_cam_vectors();
    }

    // Destructor
    ~Camera() = default;

    // Getters
    glm::vec3 get_position() {
        return m_position;
    }

    glm::mat4 get_view_matrix() {
        update_cam_vectors();
        m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
        return m_view_matrix;
    }

    // Update functions
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
                if (camera_constraint(delta_time, direction)) {
                    m_position += m_front * m_movement_speed * delta_time;
                }
                break;
            // Move backward
            case 'B':
                if (camera_constraint(delta_time, direction)) {
                    m_position -= m_front * m_movement_speed * delta_time;
                }
                break;
            // Move left
            case 'L':
                if (camera_constraint(delta_time, direction)) {
                    m_position -= m_right * m_movement_speed * delta_time;
                }
                break;
            // Move right
            case 'R':
                if (camera_constraint(delta_time, direction)) {
                    m_position += m_right * m_movement_speed * delta_time;
                }
                break;
            default:
                break;
        }
        std::cout << "X: " << m_position.x << "\t Z: " << m_position.z << "\n";
    }

    void update_user_input(float &delta_time, char direction, double &mouse_x_offset, double &mouse_y_offset) {
        update_mouse_input(delta_time, mouse_x_offset, mouse_y_offset);
        update_key_input(delta_time, direction);
    }
};