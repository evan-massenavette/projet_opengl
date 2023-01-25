#include <cstdint>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "controls.hpp"
#include "gl_wrappers/shader_manager.hpp"
#include "gl_wrappers/shader_program_manager.hpp"
#include "scene/scene.hpp"
#include "shader_structs/directional_light.hpp"

#include "renderer.hpp"

Renderer::Renderer(const App& app, const Scene& scene, const Camera& camera)
    : _app(app), _scene(scene), _camera(camera) {
  // Depth test (closest will be displayed)
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Cull triangles that are not facing the camera
  // glEnable(GL_CULL_FACE);

  // Anti-aliasing
  glEnable(GL_MULTISAMPLE);

  // Background color
  glClearColor(_scene.backgroundColor.r, _scene.backgroundColor.g,
               _scene.backgroundColor.b, _scene.backgroundColor.a);

  // Load shaders
  _loadMainShaderProgram();
  _loadDepthShaderProgram();

  // Create UBOs for shaders structs
  _createShaderStructsUBOs();

  // Create shadows framebuffers
  _createDepthFBOs();
}

void Renderer::_loadMainShaderProgram() {
  // Create shader program
  auto& mainProgram = ShaderProgramManager::getInstance().createShaderProgram(
      ShaderProgramKeys::main());

  // Load shaders
  ShaderManager& shaderManager = ShaderManager::getInstance();
  shaderManager.loadVertexShader(ShaderProgramKeys::main(),
                                 "shaders/main.vert");
  shaderManager.loadFragmentShader(ShaderProgramKeys::main(),
                                   "shaders/main.frag");
  shaderManager.loadGeometryShader(ShaderProgramKeys::main(),
                                   "shaders/main.geom");

  // Add loaded shaders to the program
  mainProgram.addShaderToProgram(
      shaderManager.getVertexShader(ShaderProgramKeys::main()));
  mainProgram.addShaderToProgram(
      shaderManager.getFragmentShader(ShaderProgramKeys::main()));
  mainProgram.addShaderToProgram(
      shaderManager.getGeometryShader(ShaderProgramKeys::main()));

  // Link program
  mainProgram.linkProgram();
}

void Renderer::_loadDepthShaderProgram() {
  // Create shader program
  auto& depthProgram = ShaderProgramManager::getInstance().createShaderProgram(
      ShaderProgramKeys::depth());

  // Load shaders
  ShaderManager& shaderManager = ShaderManager::getInstance();
  shaderManager.loadVertexShader(ShaderProgramKeys::depth(),
                                 "shaders/depth.vert");
  shaderManager.loadGeometryShader(ShaderProgramKeys::depth(),
                                   "shaders/depth.geom");
  shaderManager.loadFragmentShader(ShaderProgramKeys::depth(),
                                   "shaders/depth.frag");

  // Add loaded shaders to the program
  depthProgram.addShaderToProgram(
      shaderManager.getVertexShader(ShaderProgramKeys::depth()));
  depthProgram.addShaderToProgram(
      shaderManager.getGeometryShader(ShaderProgramKeys::depth()));
  depthProgram.addShaderToProgram(
      shaderManager.getFragmentShader(ShaderProgramKeys::depth()));

  // Link program
  depthProgram.linkProgram();
}

void Renderer::_createShaderStructsUBOs() {
  auto& mainProgram = ShaderProgramManager::getInstance().getShaderProgram(
      ShaderProgramKeys::main());

  // Ambient lights UBO
  _uboAmbientLights.createUBO(
      Scene::MAX_AMBIENT_LIGHTS *
      shader_structs::AmbientLight::getDataSizeStd140());
  _uboAmbientLights.bindBufferBaseToBindingPoint(
      UniformBlockBindingPoints::AMBIENT_LIGHTS);
  mainProgram.bindUniformBlockToBindingPoint(
      "AmbientLightsBlock", UniformBlockBindingPoints::AMBIENT_LIGHTS);

  // Directional lights UBO
  _uboDirectionalLights.createUBO(
      Scene::MAX_DIRECTIONAL_LIGHTS *
      shader_structs::DirectionalLight::getDataSizeStd140());
  _uboDirectionalLights.bindBufferBaseToBindingPoint(
      UniformBlockBindingPoints::DIRECTIONAL_LIGHTS);
  mainProgram.bindUniformBlockToBindingPoint(
      "DirectionalLightsBlock", UniformBlockBindingPoints::DIRECTIONAL_LIGHTS);

  // Point lights UBO
  _uboPointLights.createUBO(Scene::MAX_POINT_LIGHTS *
                            shader_structs::PointLight::getDataSizeStd140());
  _uboPointLights.bindBufferBaseToBindingPoint(
      UniformBlockBindingPoints::POINT_LIGHTS);
  mainProgram.bindUniformBlockToBindingPoint(
      "PointLightsBlock", UniformBlockBindingPoints::POINT_LIGHTS);
}

void Renderer::_createDepthFBOs() {
  // Point Lights

  // Create depth frame buffer
  glGenFramebuffers(1, &_depthFrameBufferID);

  // Create depth texture cube map
  _depthTextureCubeMap.create(_shadowMapSize, _shadowMapSize,
                              GL_DEPTH_COMPONENT);

  // Attach depth texture cube map to frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, _depthFrameBufferID);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthFrameBufferID,
                       0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::array<glm::mat4, 6> Renderer::_getCubeMapViewMatrices(
    const glm::vec3& position) {
  static const std::array<glm::mat4, 6> viewMatrices = {
      glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0),
                  glm::vec3(0.0, -1.0, 0.0)),
      glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0),
                  glm::vec3(0.0, -1.0, 0.0)),
      glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0),
                  glm::vec3(0.0, 0.0, 1.0)),
      glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0),
                  glm::vec3(0.0, 0.0, -1.0)),
      glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0),
                  glm::vec3(0.0, -1.0, 0.0)),
      glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0),
                  glm::vec3(0.0, -1.0, 0.0)),
  };

  return viewMatrices;
}

void Renderer::_sendShaderStructsToProgram() {
  auto& mainProgram = ShaderProgramManager::getInstance().getShaderProgram(
      ShaderProgramKeys::main());

  // Variables used when sending UBOs
  GLsizeiptr offset = 0;
  size_t size = 0;

  // Send Ambient Lights
  _uboAmbientLights.bindUBO();
  offset = 0;
  // Send count
  GLint ambientLightsCount = (GLint)_scene.ambientLights.size();
  _uboAmbientLights.setBufferData(offset, &ambientLightsCount, sizeof(GLint));
  offset += sizeof(glm::vec4);
  // Send data
  size = shader_structs::AmbientLight::getDataSizeStd140();
  for (const auto& light : _scene.ambientLights) {
    _uboAmbientLights.setBufferData(offset, light.getDataPointer(), size);
    offset += size;
  }
  _uboAmbientLights.unbindUBO();

  // Send Directional Lights
  _uboDirectionalLights.bindUBO();
  offset = 0;
  // Send count
  GLint directionalLightsCount = (GLint)_scene.directionalLights.size();
  _uboDirectionalLights.setBufferData(offset, &directionalLightsCount,
                                      sizeof(GLint));
  offset += sizeof(glm::vec4);
  // Send data
  size = shader_structs::DirectionalLight::getDataSizeStd140();
  for (const auto& light : _scene.directionalLights) {
    _uboDirectionalLights.setBufferData(offset, light.getDataPointer(), size);
    offset += size;
  }
  _uboDirectionalLights.unbindUBO();

  // // Send Point Lights
  _uboPointLights.bindUBO();
  offset = 0;
  // Send count
  GLint pointLightsCount = (GLint)_scene.pointLights.size();
  _uboPointLights.setBufferData(offset, &pointLightsCount, sizeof(GLint));
  offset += sizeof(glm::vec4);
  // Send data
  size = shader_structs::PointLight::getDataSizeStd140();
  for (const auto& light : _scene.pointLights) {
    _uboPointLights.setBufferData(offset, light.getDataPointer(), size);
    offset += size;
  }
  _uboPointLights.unbindUBO();
}

void Renderer::_drawScene(RenderPass pass) {
  for (const auto& object : _scene.objects) {
    object->draw(pass);
  }
}

void Renderer::update() {
  // Lights depth maps pass

  // Get shader program
  auto& depthProgram = ShaderProgramManager::getInstance().getShaderProgram(
      ShaderProgramKeys::depth());
  depthProgram.useProgram();

  // Set OpenGL viewport to shadowmap size
  glViewport(0, 0, _shadowMapSize, _shadowMapSize);

  // Calculate projection matrix
  const float vFov = 90.0f;
  const float aspectRatio = 1;
  const float zNear = 0.1f;
  const float zFar = 1500.0f;
  const auto projectionMatrix =
      glm::perspective(glm::radians(vFov), aspectRatio, zNear, zFar);

  // Send projection matrix and far plane uniforms
  depthProgram[ShaderConstants::projectionMatrix()] = projectionMatrix;
  depthProgram[ShaderConstants::farPlane()] = zFar;

  // Point lights shadows
  for (size_t i = 0; i < _scene.pointLights.size(); i++) {
    // Needed vars
    const auto& light = _scene.pointLights[i];

    // Get view matrix and send it to shader
    const auto viewMatrices = _getCubeMapViewMatrices(light.position);
    depthProgram[ShaderConstants::cubeMapViewMatrices()].set(
        viewMatrices.data(), (GLsizei)viewMatrices.size());

    // Send light position
    depthProgram[ShaderConstants::lightWorldPos()] = light.position;

    // Bind this light's depth frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, _depthFrameBufferID);

    // Clear depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    // Draw the scene
    _drawScene(RenderPass::Depth);

    // Unbind this light's depth frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  // Main pass

  // Get shader program
  auto& mainProgram = ShaderProgramManager::getInstance().getShaderProgram(
      ShaderProgramKeys::main());
  mainProgram.useProgram();

  // Send uniforms to shader
  mainProgram[ShaderConstants::projectionMatrix()] = _app.getProjectionMatrix();
  mainProgram[ShaderConstants::viewMatrix()] = _camera.getViewMatrix();
  mainProgram[ShaderConstants::cameraWorldPos()] = _camera.getPosition();

  // Depth
  mainProgram[ShaderConstants::farPlane()] = zFar;
  GLint depthCubeMapTextureUnit = 1;
  mainProgram[ShaderConstants::depthSampler()] = depthCubeMapTextureUnit;
  _depthTextureCubeMap.bind(depthCubeMapTextureUnit);

  // Send structs to shaders
  _sendShaderStructsToProgram();

  // Bind default frame buffer for main pass
  FrameBuffer::Default::bindAsReadAndDraw();

  // Set OpenGL's viewport size to app's window size
  FrameBuffer::Default::setFullViewport(_app);

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw all objects in the scene
  _drawScene(RenderPass::Main);
}
