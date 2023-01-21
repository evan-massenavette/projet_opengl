#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "../buffer_objects/vertex_buffer_object.hpp"
#include "../texture.hpp"
#include "../uniform.hpp"

#include "../vertex.hpp"

class GLObject {
 public:
  VertexBufferObject vbo;
  GLuint vao;
  std::vector<Vertex> vertices;
  Texture texture;
  glm::mat4 modelMatrix = glm::mat4(1);

  // Regular constructor
  GLObject(const char* modelFilepath, const char* textureFilepath);

  // Disable copy constructors
  GLObject(const GLObject&) = delete;
  GLObject& operator=(const GLObject&) = delete;

  ~GLObject();

  void draw(Uniform textureSampler);

 private:
  void _loadModel(const char* modelFilepath, const char* textureFilepath);
  void _bufferData();
};

#endif