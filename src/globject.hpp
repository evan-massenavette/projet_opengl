#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include <vector>

#include "vertex.hpp"

class GLObject {
public:
  GLuint vbo;
  GLuint vao;
  std::vector<Vertex> vertices;

  GLObject(const char *modelFilename);
  GLObject(const GLObject &) = delete;

  ~GLObject();

  void draw();

private:
  std::string _modelsDirPath = "models/";

  void _loadModel(const char *modelFilename);
  void _bufferData();
};

#endif