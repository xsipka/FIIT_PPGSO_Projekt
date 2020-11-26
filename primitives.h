#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class Shapes {
private:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;

public:
    // Constructor
    Shapes() = default;

    // Destructor
    virtual ~Shapes() = default;

    // Loads vertex data and indices into vectors
    void load_vertices(Vertex *vertices, unsigned vertices_num, GLuint *indices, unsigned indices_num) {
        // Loads vertices
        for (size_t i = 0; i < vertices_num; i++) {
            m_vertices.push_back(vertices[i]);
        }
        // Loads indices
        for (size_t i = 0; i < indices_num; i++) {
            m_indices.push_back(indices[i]);
        }
    }

    // Getter functions
    Vertex* get_vertices() { return m_vertices.data(); }
    unsigned get_vertices_num() { return m_vertices.size(); }

    GLuint* get_indices() { return m_indices.data(); }
    unsigned get_indices_num() { return m_indices.size(); }
};


class Cube : public Shapes {
public:
    Cube() : Shapes() {

        Vertex vertices[] = {
            { glm::vec3(-0.5f, 0.5f, 0.5f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f), },
            { glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f), },
            { glm::vec3(0.5f, -0.5f, 0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f), },
            { glm::vec3(0.5f, 0.5f, 0.5f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f), },

            { glm::vec3(0.5f, 0.5f, -0.5f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f), },
            { glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f), },
            { glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f), },
            { glm::vec3(-0.5f, 0.5f, -0.5f),	glm::vec3(1.f, 1.f, 0.f),	glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f) } };

        unsigned vertices_num = sizeof(vertices) / sizeof(Vertex);
        GLuint indices[] = {
                0, 1, 2,
                0, 2, 3,
                7, 6, 1,
                7, 1, 0,
                4, 5, 6,
                4, 6, 7,
                3, 2, 5,
                3, 5, 4,
                1, 2, 6,
                6, 5, 2,
                0, 3, 7,
                3, 7, 4
                };

        unsigned indices_num = sizeof(indices) / sizeof(GLuint);

        load_vertices(vertices, vertices_num, indices, indices_num);
    }
};

class Square : public Shapes {
public:
    Square() : Shapes() {

        Vertex vertices[] = {
                { glm::vec3(-0.5f, 0.f, 0.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), },
                { glm::vec3(-0.5f, 0.f, -0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f),},
                { glm::vec3(0.5f, 0.f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f), },
                { glm::vec3(0.5f, 0.f, 0.5f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, 1.f) } };

        unsigned vertices_num = sizeof(vertices) / sizeof(Vertex);
        GLuint indices[] = { 0, 1, 2, 0, 2, 3};
        unsigned indices_num = sizeof(indices) / sizeof(GLuint);

        load_vertices(vertices, vertices_num, indices, indices_num);
    }
};
