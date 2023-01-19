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

  // Directional lights UBO
  _uboDirectionalLights.createUBO(
      Scene::MAX_DIRECTIONAL_LIGHTS *
      shader_structs::DirectionalLight::getDataSizeStd140());
  _uboDirectionalLights.bindBufferBaseToBindingPoint(
      UniformBlockBindingPoints::DIRECTIONAL_LIGHTS);
  _shaderProgram.bindUniformBlockToBindingPoint(
      "DirectionalLightsBlock", UniformBlockBindingPoints::DIRECTIONAL_LIGHTS);
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

Renderer::~Renderer() {}

void Renderer::update(const glm::mat4& projection, const glm::mat4& view) {
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Send Matrices to shader
  _shaderProgram[ShaderConstants::projectionMatrix()] = projection;
  _shaderProgram[ShaderConstants::viewMatrix()] = view;

  // Send Directional Lights to shader
  GLsizeiptr offset = 0;
  // Send count
  GLint directionalLightsCount = _scene.directionalLights.size();
  _uboDirectionalLights.setBufferData(offset, &directionalLightsCount,
                                      sizeof(GLint));
  offset += sizeof(glm::vec4);
  // Send data
  for (const auto& light : _scene.directionalLights) {
    auto size = light.getDataSizeStd140();
    _uboDirectionalLights.setBufferData(offset, light.getDataPointer(), size);
    offset += size;
  }
  _uboDirectionalLights.unbindUBO();

  for (auto& object : _scene.objects) {
    // Send the model and normal matrices
    _shaderProgram.setModelAndNormalMatrix(object->modelMatrix);

    // Draw the object
    object->draw(_shaderProgram["textureSampler"]);
  }
}
