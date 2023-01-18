#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include "../vertex.hpp"

class GLObject {
public:
  GLuint vbo;
  GLuint vao;
  std::vector<Vertex> vertices;
  GLuint texture;
  glm::mat4 model = glm::mat4(1);

  // Regular constructor
  GLObject(const char *modelFilepath, const char *textureFilepath);

  // Disable copy constructors
  GLObject(const GLObject &) = delete;
  GLObject &operator=(const GLObject &) = delete;

  ~GLObject();

  void draw(const GLuint textureSampler);

private:
  void _loadModel(const char *modelFilepath, const char *textureFilepath);
  void _bufferData();
};

#endif