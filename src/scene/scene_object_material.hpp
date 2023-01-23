#ifndef SCENE_OBJECT_MATERIAL_HPP
#define SCENE_OBJECT_MATERIAL_HPP

#include <memory>
#include <vector>

#include <glad/glad.h>

#include "../gl_wrappers/texture.hpp"
#include "../gl_wrappers/vertex_buffer_object.hpp"
#include "../shader_structs/material.hpp"
#include "vertex.hpp"

class SceneObjectMaterial {
 public:
  VertexBufferObject vbo;
  GLuint vao;
  std::vector<Vertex> vertices;
  std::shared_ptr<Texture> texture;
  shader_structs::Material material;

  SceneObjectMaterial(shader_structs::Material material);
  ~SceneObjectMaterial();

  // Disable copy constructor
  SceneObjectMaterial(const SceneObjectMaterial&) = delete;
  SceneObjectMaterial& operator=(const SceneObjectMaterial&) = delete;

  void bufferData();
  void draw();
};
#endif