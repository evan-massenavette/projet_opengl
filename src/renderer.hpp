#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "app.hpp"
#include "camera/camera.hpp"
#include "gl_wrappers/shader_program.hpp"
#include "gl_wrappers/uniform_buffer_object.hpp"
#include "render_pass.hpp"
#include "scene/scene.hpp"

class Renderer {
 public:
  Renderer(const App& app, const Scene& scene, const Camera& camera);
  void update();

 private:
  const App& _app;
  const Scene& _scene;
  const Camera& _camera;

  UniformBufferObject _uboAmbientLights;
  UniformBufferObject _uboDirectionalLights;
  UniformBufferObject _uboPointLights;

  void _loadShaderProgram();
  void _createShaderStructsUBOs();
  void _sendShaderStructsToProgram();
  void _drawScene(RenderPass pass);
};

#endif