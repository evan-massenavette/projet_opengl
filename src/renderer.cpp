#include <cstdint>
#include <iostream>

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
  _loadShaderProgram();

  // Create UBOs for shaders structs
  _createShaderStructsUBOs();
}

void Renderer::_loadShaderProgram() {
  // Create shader program
  auto& programManager = ShaderProgramManager::getInstance();
  auto& mainProgram =
      programManager.createShaderProgram(ShaderProgramKeys::main());

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

  // Link program and use it
  mainProgram.linkProgram();
  mainProgram.useProgram();
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


void Renderer::_drawScene(RenderPass pass) {
  for (const auto& object : _scene.objects) {
    object->draw(pass);
  }
}

void Renderer::update() {

  // Get shader program
  auto& mainProgram = ShaderProgramManager::getInstance().getShaderProgram(
      ShaderProgramKeys::main());

  // Send uniforms to shader
  mainProgram[ShaderConstants::projectionMatrix()] = projectionMatrix;
  mainProgram[ShaderConstants::viewMatrix()] = camera.getViewMatrix();
  mainProgram[ShaderConstants::cameraWorldPos()] = camera.getPosition();

  _sendShaderStructsToProgram();

  // Draw all objects in the scene
  _drawScene(RenderPass::Main);
}

void Renderer::_sendShaderStructsToProgram() {
  auto& mainProgram = ShaderProgramManager::getInstance().getShaderProgram(
      ShaderProgramKeys::main());

  // Send Material
  // TODO: Have GLObjects send their own material in their draw() method
  // auto material = shader_structs::Material::defaultOne();
  // material.setUniform(mainProgram, ShaderConstants::material());

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
