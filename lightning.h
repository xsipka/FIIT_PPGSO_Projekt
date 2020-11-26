
class Lightning {
protected:
    glm::vec3 m_position;
    glm::vec3 m_color;
    float m_intensity;
    int m_id;

public:
    // Constructor
    Lightning(glm::vec3 position, glm::vec3 color, float intensity, int id) {
        m_position = position;
        m_color = color;
        m_intensity = intensity;
        m_id = id;
    }

    // Destructor
    ~Lightning() = default;

    // Getters, setters & modifiers
    void set_position(glm::vec3 position) { m_position = position; }
    void set_color(glm::vec3 color) { m_color = color; }
    void set_intensity(float intensity) { m_intensity = intensity; }

    void move_light(glm::vec3 position) { m_position += position; }
    void adjust_color(glm::vec3 color) { m_color += color; }

    glm::vec3 get_color() const { return m_color; }
    glm::vec3 get_position() const { return m_position; }

    float get_intensity() const { return m_intensity; }
    int get_id() const { return m_id; }

    // Sends point light values to shader
    void send_to_shader(Shader &program, const int id) {

        std::string str_id = std::to_string(id);

        std::string position_str = "lights[" + str_id +"].position";
        char* position = const_cast<char *>(position_str.c_str());

        std::string color_str = "lights[" + str_id +"].color";
        char* color = const_cast<char *>(color_str.c_str());

        std::string intensity_str = "lights[" + str_id +"].intensity";
        char* intensity = const_cast<char *>(intensity_str.c_str());

        program.set_gl_vec3(m_position, position);
        program.set_gl_vec3(m_color, color);
        program.set_gl_float(m_intensity, intensity);
    }
};
