#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "buffer_objects/uniform_buffer_object.hpp"
#include "camera.hpp"
#include "shader_program.hpp"

#include "scene/scene.hpp"

class Renderer {
 public:
  Renderer(Scene& scene);
  ~Renderer();
  void update(const glm::mat4& projectionMatrix, const Camera& camera);

 private:
  Scene& _scene;

  const std::string MAIN_PROGRAM_KEY = "main";

  UniformBufferObject _uboAmbientLights;
  UniformBufferObject _uboDirectionalLights;
  UniformBufferObject _uboPointLights;

  void _loadShaderProgram();
  void _createShaderStructsUBOs();
  void _sendShaderStructsToProgram();
};

#endif