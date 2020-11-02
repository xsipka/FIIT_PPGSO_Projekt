#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include <iostream>
#include <fstream>
#include <string>


class Shader {
private:
    GLuint m_id{};

    // returns string containing source code for shader
    static std::string load_shader_file(const std::string &file_name) {

        std::string temp = "";
        std::string source_code = "";

        // Vertex shader
        std::ifstream input_file;
        input_file.open(R"(shaders\)" + file_name);

        if (input_file.is_open()) {
            while (std::getline(input_file, temp)) {
                source_code += temp + "\n";
            }
        } else {
            std::cout << "Error in shaders.h, load_shader_file(): " << file_name << " couldn't be opened ...\n";
            exit(1);
        }
        return source_code;
    }

    // returns shader
    static GLuint load_shader(GLenum shader_type, const std::string &file_name) {

        char info_log[512];
        GLint checker = 0;

        GLuint shader = glCreateShader(shader_type);
        std::string data_str = Shader::load_shader_file(file_name);
        const GLchar *data = data_str.c_str();
        glShaderSource(shader, 1, &data, nullptr);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &checker);
        if (!checker) {
            glGetShaderInfoLog(shader, 512, nullptr, info_log);
            std::cout << "Error in shaders.h, load_shader(): " << info_log << "\n";
            exit(1);
        }
        return shader;
    }

    // links shaders to program (vertex, fragment)
    void link_shaders(GLuint vertex_shader, GLuint frag_shader) {

        char info_log[512];
        GLint checker = 0;
        m_id = glCreateProgram();
        glUseProgram(m_id);

        glAttachShader(m_id, vertex_shader);
        glAttachShader(m_id, frag_shader);

        glLinkProgram(m_id);
        glGetProgramiv(m_id, GL_LINK_STATUS, &checker);

        if (!checker) {
            glGetProgramInfoLog(m_id, 512, nullptr, info_log);
            std::cerr << "Error in shaders.h, link_shader(): " << info_log << "\n";
            exit(1);
        }

        glUseProgram(0);
        glDeleteShader(vertex_shader);
        glDeleteShader(frag_shader);
    }


public:
    // Constructor
    Shader(const std::string& vertex_file, const std::string& frag_file) {
        GLuint vertex_shader;
        GLuint frag_shader;

        vertex_shader = load_shader(GL_VERTEX_SHADER, vertex_file);
        frag_shader = load_shader(GL_FRAGMENT_SHADER, frag_file);

        this->link_shaders(vertex_shader, frag_shader);
    }

    //Destructor
    ~Shader() {
        glDeleteProgram(m_id);
    }

    // Use this program
    void use_program() const {
        glUseProgram(m_id);
    }

    // Unuse this program
    static void set_to_null() {
        glUseProgram(0);
    }

    // Sets uniforms (matrices, vectors, other variables)
    void set_gl_mat4(glm::mat4 values, const GLchar *name, GLboolean transpose) const {

        use_program();
        glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, transpose, glm::value_ptr(values));
        Shader::set_to_null();
    }

    void set_gl_vec4(glm::fvec4 values, const GLchar *name) const {

        use_program();
        glUniform4fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(values));
        Shader::set_to_null();
    }

    void set_gl_vec3(glm::fvec3 values, const GLchar *name) const {

        use_program();
        glUniform3fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(values));
        Shader::set_to_null();
    }

    void set_gl_float(GLfloat value, const GLchar *name) const {

        use_program();
        glUniform1f(glGetUniformLocation(m_id, name), value);
        Shader::set_to_null();
    }

    void set_gl_int(GLint value, const GLchar *name) const {

        use_program();
        glUniform1i(glGetUniformLocation(m_id, name), value);
        Shader::set_to_null();
    }
};