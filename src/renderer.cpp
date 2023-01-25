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
  _createShadowsFramebuffers();
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

void Renderer::_createShadowsFramebuffers() {
  // Point Lights
  for (const auto& light : _scene.pointLights) {
    auto depthFBO = std::make_unique<FrameBuffer>();
    depthFBO->create(_shadowMapSize, _shadowMapSize);
    depthFBO->bindAsReadAndDraw();
    depthFBO->addTextureCubeMap(GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
    _fbosDepthPointLights.emplace_back(std::move(depthFBO));
  }
}

glm::mat4 Renderer::_getCubeMapViewMatrix(size_t index,
                                          const glm::vec3& position) {
  if (index >= 6) {
    throw std::runtime_error(
        "Cannot get cube map view matrix with an index >= 6");
  }

  static const std::vector<glm::mat4> viewMatrices = {
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

  return viewMatrices[index];
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
    const auto& depthFBO = _fbosDepthPointLights[i];

    // Get view matrix and send it to shader
    const auto viewMatrix = _getCubeMapViewMatrix(i, light.position);
    depthProgram[ShaderConstants::viewMatrix()] = viewMatrix;

    // Send light position as camera position
    depthProgram[ShaderConstants::cameraWorldPos()] = light.position;

    // Bind this light's depth FBO
    depthFBO->bindAsReadAndDraw();

    // Clear depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    _drawScene(RenderPass::Depth);
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

  // TEMP
  mainProgram[ShaderConstants::farPlane()] = zFar;
  mainProgram[ShaderConstants::depthSampler()] = 1;

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
