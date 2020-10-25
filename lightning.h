
class Lightning {
protected:
    glm::vec3 m_color;
    float m_intensity;

public:
    // Constructor
    Lightning(glm::vec3 color, float intensity) {
        m_color = color;
        m_intensity = intensity;
    }

    // Destructor
    ~Lightning() = default;
};


class PointLight : public Lightning {
protected:
    glm::vec3 m_position;

public:
    // Constructor
    PointLight(glm::vec3 position, glm::vec3 color, float intensity) : Lightning(color, intensity) {
        m_position = position;
    }

    // Destructor
    ~PointLight() = default;

    // Getters and setters
    void set_position(glm::vec3 position) {
        m_position = position;
    }

    // Sends point light values to shader
    void send_to_shader(Shader &program) {
        program.set_gl_vec3(m_position, "point_light.position");
        program.set_gl_vec3(m_color, "point_light.color");
        program.set_gl_float(m_intensity, "point_light.intensity");
    }
};