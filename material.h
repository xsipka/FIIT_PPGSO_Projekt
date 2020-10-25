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
    GLint m_diffuse_texture;
    GLint m_specular_texture;

public:
    // Constructor
    Material (
    glm::vec3 ambient,
    glm::vec3 diffuse,
    glm::vec3 specular,
    GLint diffuse_texture,
    GLint specular_texture) {

        m_ambient = ambient;
        m_diffuse = diffuse;
        m_specular = specular;
        m_diffuse_texture = diffuse_texture;
        m_specular_texture = specular_texture;
    }

    // Destructor
    ~Material() = default;

    // Sends all values to shader
    void send_to_shader(Shader &program) {

        program.set_gl_vec3(m_ambient, "material.ambient");
        program.set_gl_vec3(m_diffuse, "material.diffuse");
        program.set_gl_vec3(m_specular, "material.specular");
        program.set_gl_int(m_diffuse_texture, "material.diffuse_texture");
        program.set_gl_int(m_specular_texture, "material.specular_texture");
    }
};