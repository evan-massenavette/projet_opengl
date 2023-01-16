#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include "controls.hpp"
#include "globject.hpp"
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

  // Get MVP
  _mvp = glGetUniformLocation(_program, "MVP");

  // Texture
  _textureSampler = glGetUniformLocation(_program, "textureSampler");

  // Load model
  std::shared_ptr<GLObject> model(new GLObject(
      "models/rusty_barrel/rusty_barrel.obj",
      "models/rusty_barrel/textures/varil_low_lambert1_BaseColor.bmp"));
  _models.push_back(model);
}

Renderer::~Renderer() {
  // Cleanup
  glDeleteProgram(_program);
}

void Renderer::update(const glm::mat4 &mvp) {
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use our shader
  glUseProgram(_program);

  // Send MVP to the currently bound shader
  glUniformMatrix4fv(_mvp, 1, GL_FALSE, &mvp[0][0]);

  for (auto &model : _models) {
    model->draw(_textureSampler);
  }
}
