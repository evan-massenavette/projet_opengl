#include <cstdint>

#include "controls.hpp"
#include "scene/scene.hpp"
#include "shaders.hpp"

#include "renderer.hpp"

Renderer::Renderer() {
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  // Cull triangles which normal is not towards the camera
  // glEnable(GL_CULL_FACE);

  // Enable anti-aliasing
  glEnable(GL_MULTISAMPLE);

  // Background color
  glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

  // Load shaders
  _program = Shaders::createProgram("shaders/TextureTransform.vert",
                                    "shaders/Texture.frag");
  glUseProgram(_program);

  // Matrices
  _projection = glGetUniformLocation(_program, "projection");
  _view = glGetUniformLocation(_program, "view");
  _model = glGetUniformLocation(_program, "model");

  // Directional lights
  auto directionalLightsBufferSize =
      sizeof(GLuint) +
      Scene::MAX_NUM_DIRECTIONAL_LIGHTS * sizeof(DirectionalLight);
  _uboDirectionalLights = getUBO("Lighting", 1, directionalLightsBufferSize);

  // Texture
  _textureSampler = glGetUniformLocation(_program, "textureSampler");
}

GLuint Renderer::getUBO(const char *uniformBlockName,
                        GLuint uniformBlockBinding,
                        GLsizeiptr uniformBlockSize) {
  // Set the uniform block to point to the given binding point
  auto uniformBlockIndex = glGetUniformBlockIndex(_program, uniformBlockName);
  glUniformBlockBinding(_program, uniformBlockIndex, uniformBlockBinding);

  // Create UBO and bind it to binding point 0
  GLuint ubo;
  glGenBuffers(1, &ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize, NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, uniformBlockSize);

  return ubo;
}

Renderer::~Renderer() {
  // Cleanup
  glDeleteProgram(_program);
}

void Renderer::update(const Scene &scene, const glm::mat4 &projection,
                      const glm::mat4 &view) {
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Send Matrices to shader
  glUniformMatrix4fv(_projection, 1, GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(_view, 1, GL_FALSE, &view[0][0]);

  // Send directional lights to shader
  glBindBuffer(GL_UNIFORM_BUFFER, _uboDirectionalLights);
  GLuint numDirectionalLights = scene.directionalLights.size();
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLuint), &numDirectionalLights);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLuint),
                  numDirectionalLights * sizeof(DirectionalLight),
                  &scene.directionalLights[0]);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  for (auto &object : scene.objects) {

    // Use this object's model matrix as the model uniform
    glUniformMatrix4fv(_model, 1, GL_FALSE, &object->model[0][0]);

    // Draw the object
    object->draw(_textureSampler);
  }
}
