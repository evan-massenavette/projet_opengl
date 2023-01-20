#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>

class Texture {
 public:
  static GLuint load(const char* filepath);
};

#endif