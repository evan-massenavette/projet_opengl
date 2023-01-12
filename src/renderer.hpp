#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include <vector>

#include "globject.hpp"

class Renderer {
public:
  Renderer();
  ~Renderer();
  void update(const glm::mat4 &mvp);

private:
  GLuint _program;
  GLuint _mvp;

  GLuint _texture;
  GLuint _textureSampler;

  std::vector<std::shared_ptr<GLObject>> _models;
};

#endif