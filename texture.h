#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include <iostream>
#include <fstream>
#include <string>

// Used for loading textures
// Source: https://github.com/nothings/stb/blob/master/stb_image.h
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>


class Texture {
private:
    GLuint m_id {};
    int m_image_width {};
    int m_image_height {};
    int m_comp {};

public:
    // Constructors
    Texture(const char *file_name, GLenum texture_type, bool flip = false) {

        if (flip) {
            stbi_set_flip_vertically_on_load(true);
        }
        unsigned char *image = stbi_load(file_name, &m_image_width, &m_image_height, &m_comp, STBI_rgb_alpha);

        glGenTextures(1, &m_id);
        glBindTexture(texture_type, m_id);

        glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        if (image) {
            glTexImage2D(texture_type, 0, GL_RGBA, m_image_width, m_image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(texture_type);
        }
        else {
            std::cout << "Error in texture.h, Texture() constructor: " << file_name << " couldn't be opened ...\n";
            stbi_image_free(image);
            exit(1);
        }
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(image);
    }

    // Destructor
    ~Texture() {
        glDeleteTextures(1, &m_id);
    }

    // Returns texture id
    GLuint get_texture_id() const {
        return m_id;
    }

    // Binds texture and make it active
    void bind_texture(GLenum texture_type, unsigned texture_unit) const {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(texture_type, m_id);
    }

    // Unbinds the texture
    static void unbind_texture (GLenum texture_type) {
        glActiveTexture(0);
        glBindTexture(texture_type, 0);
    }
};