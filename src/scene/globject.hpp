#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "../buffer_objects/vertex_buffer_object.hpp"
#include "../texture.hpp"
#include "../uniform.hpp"

#include "../vertex.hpp"

#include "../scene/face.hpp"

class GLObject {
 public:
  std::vector<std::shared_ptr<Texture>> textures;
  std::vector<std::unique_ptr<Face>> faces;
  glm::mat4 modelMatrix = glm::mat4(1);

  // Regular constructor
  GLObject(const std::string& modelFilepath);

  // Disable copy constructors
  GLObject(const GLObject&) = delete;
  GLObject& operator=(const GLObject&) = delete;

  ~GLObject();

  void draw(Uniform textureSampler);

  void rotate(float angle_x, float angle_y, float angle_z);
  void translate(float distance_x, float distance_y, float distance_z);
  void scale(float factor_x, float factor_y, float factor_z);
  void scale(float factor);

 private:
  void _loadModel(const std::string& modelFilepath);
};

#endif