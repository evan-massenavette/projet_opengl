#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <array>
#include <memory>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "app.hpp"
#include "camera/camera.hpp"
#include "gl_wrappers/frame_buffer.hpp"
#include "gl_wrappers/shader_program.hpp"
#include "gl_wrappers/texture_cube_map.hpp"
#include "gl_wrappers/uniform_buffer_object.hpp"
#include "render_pass.hpp"
#include "scene/scene.hpp"

class App;

class Renderer {
 public:
  Renderer(const App& app, const Scene& scene);
  void setCamera(Camera* camera);
  void update();

 private:
  const App& _app;
  const Scene& _scene;
  Camera* _camera;

  UniformBufferObject _uboAmbientLights;
  UniformBufferObject _uboDirectionalLights;
  UniformBufferObject _uboPointLights;

  const GLsizei _shadowMapSize = 1024;
  GLuint _depthFrameBufferID;
  TextureCubeMap _depthTextureCubeMap;

  void _loadMainShaderProgram();
  void _loadDepthShaderProgram();
  void _createShaderStructsUBOs();
  void _createDepthFBOs();
  void _sendShaderStructsToProgram();
  void _drawScene(RenderPass renderPass);

  std::array<glm::mat4, 6> _getCubeMapViewMatrices(const glm::vec3& position);
};

#endif