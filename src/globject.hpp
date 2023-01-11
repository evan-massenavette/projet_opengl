#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

class GLObject {
public:
  GLuint vbo;
  GLuint vao;
  GLuint ibo;
  GLuint size;

  GLObject(
      const std::vector<Vertex3dColor> &vertices,
      [[maybe_unused]] const std::optional<std::vector<std::array<GLuint, 3>>>
          &indices);

  ~GLObject();

  void draw();
};

#endif