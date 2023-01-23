#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "../gl_wrappers/texture.hpp"
#include "../gl_wrappers/uniform.hpp"
#include "../gl_wrappers/vertex_buffer_object.hpp"
#include "object_material.hpp"
#include "vertex.hpp"

class GLObject
{
public:
  std::vector<std::unique_ptr<ObjectMaterial>> objectMaterials;
  glm::mat4 modelMatrix = glm::mat4(1);

  // Regular constructor
  GLObject(const std::string &modelFilepath);

  // Disable copy constructors
  GLObject(const GLObject &) = delete;
  GLObject &operator=(const GLObject &) = delete;

  ~GLObject();

  void draw(Uniform textureSampler);

  void rotate(float angle_x, float angle_y, float angle_z);
  void translate(float distance_x, float distance_y, float distance_z);
  void scale(float factor_x, float factor_y, float factor_z);
  void scale(float factor);

private:
  void _loadModel(const std::string &modelFilepath);
  void _bufferData();
};

#endif