#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include <iostream>
#include <fstream>
#include <string>


class Material {
private:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    GLint m_texture;

public:
    // Constructor
    Material (glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint texture) {

        m_ambient = ambient;
        m_diffuse = diffuse;
        m_specular = specular;
        m_texture = texture;
    }

    // Destructor
    ~Material() = default;

    // Sends all values to shader
    void send_to_shader(Shader &program) {

        program.set_gl_vec3(m_ambient, "material.ambient");
        program.set_gl_vec3(m_diffuse, "material.diffuse");
        program.set_gl_vec3(m_specular, "material.specular");
        program.set_gl_int(m_texture, "material.texture");
    }
};