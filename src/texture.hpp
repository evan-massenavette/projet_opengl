#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>

#include <string>

class Texture
{
public:
  static GLuint load(std::string filepath);
};

#endif