#ifndef RENDERER_HPP
#define RENDERER_HPP

class Renderer {
public:
  Renderer();
  ~Renderer();
  void update(const glm::mat4 &mvp);

private:
  GLuint _program;
  GLuint _mvp;

  GLuint _vbo;
  GLuint _ibo;
  GLuint _vao;

  GLuint _texture;
  GLuint _textureSampler;
};

#endif