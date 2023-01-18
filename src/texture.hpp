#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Texture {
public:
  static GLuint load(const char *filepath);
};

#endif