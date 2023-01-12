#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include <vector>

#include "vertex.hpp"

class GLObject {
public:
  GLuint vbo;
  GLuint vao;
  std::vector<Vertex> vertices;
  GLuint texture;

  GLObject(const char *modelFilepath, const char *textureFilepath);
  GLObject(const GLObject &) = delete;

  ~GLObject();

  void draw(const GLuint textureSampler);

private:
  void _loadModel(const char *modelFilepath, const char *textureFilepath);
  void _bufferData();
};

#endif