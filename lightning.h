
class Lightning {
protected:
    glm::vec3 m_position;
    glm::vec3 m_color;
    float m_intensity;

public:
    // Constructor
    Lightning(glm::vec3 position, glm::vec3 color, float intensity) {
        m_position = position;
        m_color = color;
        m_intensity = intensity;
    }

    // Destructor
    ~Lightning() = default;

    // Getters and setters
    void set_position(glm::vec3 position) {
        m_position = position;
    }

    void set_color(glm::vec3 color) {
        m_color = color;
    }

    void set_intensity(float intensity) {
        m_intensity = intensity;
    }

    // Sends point light values to shader
    void send_to_shader(Shader &program) {
        program.set_gl_vec3(m_position, "light.position");
        program.set_gl_vec3(m_color, "light.color");
        program.set_gl_float(m_intensity, "light.intensity");
    }
};
