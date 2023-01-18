#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include "scene/scene.hpp"

class Renderer {
public:
  Renderer();
  ~Renderer();
  void update(const Scene &scene, const glm::mat4 &projection,
              const glm::mat4 &view);

private:
  GLuint _program;

  GLuint _uboDirectionalLights;

  GLuint _projection;
  GLuint _view;
  GLuint _model;

  GLuint _textureSampler;

  GLuint getUBO(const char *uniformBlockName, GLuint uniformBlockBinding,
                GLsizeiptr uniformBlockSize);
};

#endif