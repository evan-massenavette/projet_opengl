#ifndef FACE_HPP
#define FACE_HPP

#include <memory>
#include <vector>

#include <glad/glad.h>

#include "../buffer_objects/vertex_buffer_object.hpp"
#include "../shader_structs/material.hpp"
#include "../texture.hpp"
#include "../vertex.hpp"

class Face {
 public:
  VertexBufferObject vbo;
  GLuint vao;
  std::vector<Vertex> vertices;
  std::shared_ptr<Texture> texture;
  shader_structs::Material material;

  Face(std::vector<Vertex> vertices,
       std::shared_ptr<Texture> texture,
       shader_structs::Material material);
  ~Face();

  // Disable copy constructor
  Face(const Face&) = delete;
  Face& operator=(const Face&) = delete;

  void bufferData();
  void draw(Uniform textureSampler);
};
#endif