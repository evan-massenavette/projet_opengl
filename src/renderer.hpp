#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "buffer_objects/uniform_buffer_object.hpp"
#include "shader_program.hpp"

#include "scene/scene.hpp"

class Renderer {
 public:
  Renderer(Scene& scene);
  ~Renderer();
  void update(const glm::mat4& projection, const glm::mat4& view);

 private:
  Scene& _scene;

  ShaderProgram _shaderProgram;

  UniformBufferObject _uboDirectionalLights;

  void _loadShaderProgram();
};

#endif