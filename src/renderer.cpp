#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include "controls.hpp"
#include "globject.hpp"
#include "shader.hpp"
#include "texture.hpp"

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
  _program = glCreateProgram();
  std::vector<Shader> shaders = {
      Shader("shaders/TextureTransform.vert", GL_VERTEX_SHADER),
      Shader("shaders/Texture.frag", GL_FRAGMENT_SHADER),
  };
  Shader::loadAll(_program, shaders);

  // Get MVP
  _mvp = glGetUniformLocation(_program, "MVP");

  // Texture
  _texture = Texture::load("textures/uv_test_1.bmp");
  _textureSampler = glGetUniformLocation(_program, "textureSampler");

  // Load model
  std::shared_ptr<GLObject> model(new GLObject("sofa.obj"));
  _models.push_back(model);
}

Renderer::~Renderer() {
  // Cleanup
  glDeleteProgram(_program);
  glDeleteTextures(1, &_texture);
}

void Renderer::update(const glm::mat4 &mvp) {
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use our shader
  glUseProgram(_program);

  // Send MVP to the currently bound shader
  glUniformMatrix4fv(_mvp, 1, GL_FALSE, &mvp[0][0]);

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture);
  // Set our texture sampler to use Texture Unit 0
  glUniform1i(_textureSampler, 0);

  for (auto &model : _models) {
    model->draw();
  }
}
