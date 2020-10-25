#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>



static std::vector<Vertex> load_obj_file(const char* file_name) {

    std::vector<glm::fvec3> vertices_position;
    std::vector<glm::fvec2> vertices_texcoord;
    std::vector<glm::fvec3> vertices_normal;

    std::vector<GLint> indices_position;
    std::vector<GLint> indices_texcoord;
    std::vector<GLint> indices_normal;

    std::vector<Vertex> vertices;

    std::ifstream obj_file(file_name);
    if (!obj_file.is_open()) {
        std::cout << "Error in loader.h, load_obj_file(): " << file_name << " couldn't be opened ...\n";
        exit(1);
    }

    std::string line;
    std::stringstream ss_line;
    std::string prefix;
    glm::vec3 temp_vec3;
    glm::vec2 temp_vec2;
    GLint temp_int;

    while (std::getline(obj_file, line)) {

        ss_line.clear();
        ss_line.str(line);
        ss_line >> prefix;

        if (prefix == "v") {
            ss_line >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            vertices_position.push_back(temp_vec3);
        }
        else if (prefix == "vt") {
            ss_line >> temp_vec2.x >> temp_vec2.y;
            vertices_texcoord.push_back(temp_vec2);
        }
        else if (prefix == "vn") {
            ss_line >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            vertices_normal.push_back(temp_vec3);
        }
        else if (prefix == "f") {
            int counter = 0;

            while (ss_line >> temp_int) {
                // Loading data
                if (counter == 0) {
                    indices_position.push_back(temp_int);
                }
                else if (counter == 1) {
                    indices_texcoord.push_back(temp_int);
                }
                else if (counter == 2) {
                    indices_normal.push_back(temp_int);
                }

                // Checks for delimiters between values
                if (ss_line.peek() == '/') {
                    ++counter;
                    ss_line.ignore(1, '/');
                }
                else if (ss_line.peek() == ' ') {
                    ++counter;
                    ss_line.ignore(1, ' ');
                }

                // Resets the counter
                if (counter > 2) { counter = 0; }
            }
        }

    }
    vertices.resize(indices_position.size(), Vertex());

    for (size_t i = 0; i < vertices.size(); i++) {
        vertices[i].position = vertices_position[indices_position[i] - 1];
        vertices[i].texture_coord = vertices_texcoord[indices_texcoord[i] - 1];
        vertices[i].normal = vertices_normal[indices_normal[i] - 1];
        vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
    }
    std::cout << "File loaded successfully :)\n";
    return vertices;
}
