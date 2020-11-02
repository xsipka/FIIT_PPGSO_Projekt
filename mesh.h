#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include <iostream>
#include <vector>


class Mesh {
private:
    Vertex* m_vertices;
    GLuint* m_indices;

    unsigned m_vertices_num{};
    unsigned m_indices_num{};

    GLuint m_vao{};     // Vertex array object
    GLuint m_vbo{};     // Vertex buffer object
    GLuint m_ebo{};     // Element buffer object

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_model_matrix;


    void vertex_init(Vertex *vertices, unsigned  &vertices_num, GLuint *indices, unsigned &indices_num) {

        m_vertices_num = vertices_num;
        m_indices_num = indices_num;

        glCreateVertexArrays(1, &m_vao);
        glBindVertexArray(this->m_vao);

        glCreateBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertices_num * sizeof(Vertex), vertices, GL_STATIC_DRAW);

        glCreateBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices_num * sizeof(GLuint), indices, GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        // Color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        // Texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texture_coord));
        glEnableVertexAttribArray(2);
        // Lightning something
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    void model_matrix_update() {
        m_model_matrix = glm::mat4(1.f);
        m_model_matrix = glm::translate(m_model_matrix, glm::vec3(0, 0, 0));
        m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));
        m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
        m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
        m_model_matrix = glm::translate(m_model_matrix, m_position);
        m_model_matrix = glm::scale(m_model_matrix, m_scale);
    }

    void send_to_shader(Shader *shader) {
        shader->set_gl_mat4(m_model_matrix, "model_matrix", GL_FALSE);
    }


public:
    // Constructor
    Mesh(Vertex *vertices, unsigned  vertices_num, const GLuint *indices, unsigned indices_num,
         glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)) {

        m_position = position;
        m_rotation = rotation;
        m_scale = scale;

        m_vertices_num = vertices_num;
        m_indices_num = indices_num;

        m_vertices = new Vertex[m_vertices_num];
        for (size_t i = 0; i < m_vertices_num; i++) {
            m_vertices[i] = vertices[i];
        }
        m_indices = new GLuint[m_indices_num];
        for (size_t i = 0; i < m_indices_num; i++) {
            m_indices[i] = indices[i];
        }

        vertex_init(m_vertices, m_vertices_num, m_indices, m_indices_num);
        model_matrix_update();
    }

    // Constructor for objects
    Mesh(Mesh& object) {
        m_position = object.m_position;
        m_rotation = object.m_rotation;
        m_scale = object.m_scale;

        m_vertices_num = object.m_vertices_num;
        m_indices_num = object.m_indices_num;

        m_vertices = new Vertex[m_vertices_num];
        for (size_t i = 0; i < m_vertices_num; i++) {
            m_vertices[i] = object.m_vertices[i];
        }
        m_indices = new GLuint[m_indices_num];
        for (size_t i = 0; i < m_indices_num; i++) {
            m_indices[i] = object.m_indices[i];
        }

        vertex_init(m_vertices, m_vertices_num, m_indices, m_indices_num);
        model_matrix_update();
    }


    // Destructor
    ~Mesh() {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
    }

    // Getters and setters
    void set_position(glm::vec3 position) { m_position = position; }
    void set_rotation(glm::vec3 rotation) { m_rotation = rotation; }
    void set_scale(glm::vec3 scale) { m_scale = scale; }

    glm::vec3 get_position() { return m_position; }
    glm::vec3 get_rotation() { return m_rotation; }
    glm::vec3 get_scale() { return m_scale; }

    // Change position, rotation and scaling
    void move_mesh(glm::vec3 position) { m_position += position; }
    void rotate_mesh(glm::vec3 rotation) { m_rotation += rotation; }
    void scale_mesh(glm::vec3 scale) { m_scale += scale; }


    // Updates uniforms, bind VAO and renders object
    void render_mesh(Shader *shader) {

        model_matrix_update();
        send_to_shader(shader);
        shader->use_program();

        glBindVertexArray(m_vao);

        if (m_indices_num == 0) {
            glDrawArrays(GL_TRIANGLES, 0, m_vertices_num);
        } else {
            glDrawElements(GL_TRIANGLES, m_indices_num, GL_UNSIGNED_INT, nullptr);
        }

        glBindVertexArray(0);
        Texture::unbind_texture(GL_TEXTURE_2D);
        glUseProgram(0);
    }
};