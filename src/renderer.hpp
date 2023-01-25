#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "app.hpp"
#include "camera/camera.hpp"
#include "gl_wrappers/frame_buffer.hpp"
#include "gl_wrappers/shader_program.hpp"
#include "gl_wrappers/uniform_buffer_object.hpp"
#include "render_pass.hpp"
#include "scene/scene.hpp"

class App;

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

  std::vector<std::unique_ptr<FrameBuffer>> _fbosDepthPointLights;

  const GLsizei _shadowMapSize = 1024;

  void _loadMainShaderProgram();
  void _loadDepthShaderProgram();
  void _createShaderStructsUBOs();
  void _createShadowsFramebuffers();
  void _sendShaderStructsToProgram();
  void _drawScene(RenderPass pass);
};

#endif