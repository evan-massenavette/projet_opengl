#include <cstdint>
#include <iostream>

#include "controls.hpp"
#include "scene/scene.hpp"
#include "shader_program_manager.hpp"
#include "shader_structs/directional_light.hpp"

#include "renderer.hpp"

Renderer::Renderer(Scene& scene) : _scene(scene) {
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  // Cull triangles which normal is not towards the camera
  // glEnable(GL_CULL_FACE);

  // Enable anti-aliasing
  glEnable(GL_MULTISAMPLE);

  // Background color
  glClearColor(scene.backgroundColor.r, scene.backgroundColor.g,
               scene.backgroundColor.b, scene.backgroundColor.a);

  // Load shaders
  _loadShaderProgram();

  // Create UBOs for shaders structs
  _createShaderStructsUBOs();
}

void Renderer::_loadShaderProgram() {
  // Create shader program using manager
  auto& programManager = ShaderProgramManager::getInstance();
  programManager.createShaderProgram("main");
  _shaderProgram = programManager.getShaderProgram("main");

  // Load vert and frag shaders
  Shader vert;
  Shader frag;
  vert.loadShaderFromFile("shaders/TextureTransform.vert", GL_VERTEX_SHADER);
  frag.loadShaderFromFile("shaders/Texture.frag", GL_FRAGMENT_SHADER);

  if (!(vert.isCompiled() && frag.isCompiled())) {
    exit(EXIT_FAILURE);
  }

  _shaderProgram.addShaderToProgram(vert);
  _shaderProgram.addShaderToProgram(frag);

  _shaderProgram.linkProgram();
  _shaderProgram.useProgram();
}

void Renderer::_createShaderStructsUBOs() {
  // Ambient lights UBO
  _uboAmbientLights.createUBO(
      Scene::MAX_AMBIENT_LIGHTS *
      shader_structs::AmbientLight::getDataSizeStd140());
  _uboAmbientLights.bindBufferBaseToBindingPoint(
      UniformBlockBindingPoints::AMBIENT_LIGHTS);
  _shaderProgram.bindUniformBlockToBindingPoint(
      "AmbientLightsBlock", UniformBlockBindingPoints::AMBIENT_LIGHTS);

  // Directional lights UBO
  _uboDirectionalLights.createUBO(
      Scene::MAX_DIRECTIONAL_LIGHTS *
      shader_structs::DirectionalLight::getDataSizeStd140());
  _uboDirectionalLights.bindBufferBaseToBindingPoint(
      UniformBlockBindingPoints::DIRECTIONAL_LIGHTS);
  _shaderProgram.bindUniformBlockToBindingPoint(
      "DirectionalLightsBlock", UniformBlockBindingPoints::DIRECTIONAL_LIGHTS);

  // Point lights UBO
  _uboPointLights.createUBO(Scene::MAX_POINT_LIGHTS *
                            shader_structs::PointLight::getDataSizeStd140());
  _uboPointLights.bindBufferBaseToBindingPoint(
      UniformBlockBindingPoints::POINT_LIGHTS);
  _shaderProgram.bindUniformBlockToBindingPoint(
      "PointLightsBlock", UniformBlockBindingPoints::POINT_LIGHTS);
}

Renderer::~Renderer() {}

void Renderer::update(const glm::mat4& projection, const glm::mat4& view) {
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Send Matrices to shader
  _shaderProgram[ShaderConstants::projectionMatrix()] = projection;
  _shaderProgram[ShaderConstants::viewMatrix()] = view;

  _sendShaderStructsToProgram();

  for (auto& object : _scene.objects) {
    // Send the model and normal matrices
    _shaderProgram.setModelAndNormalMatrix(object->modelMatrix);

    // Draw the object
    object->draw(_shaderProgram["textureSampler"]);
  }
}

void Renderer::_sendShaderStructsToProgram() {
  // Send Material
  // TODO: Have GLObjects send their own material in their draw() method
  auto material = shader_structs::Material::default();
  material.setUniform(_shaderProgram, "material");

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
