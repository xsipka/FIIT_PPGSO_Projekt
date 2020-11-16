#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "vertex.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include "material.h"
#include "primitives.h"
#include "camera.h"
#include "model.h"
#include "loader.h"
#include "lightning.h"
#include "club.h"
#include "player.h"


// Used for loading textures
// Source: https://github.com/nothings/stb/blob/master/stb_image.h
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image/stb_image.h>