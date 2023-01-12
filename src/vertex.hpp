#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/glm.hpp>

class Vertex {
public:
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
  Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv);
};

class Vertex2dColor {
public:
  glm::vec2 position;
  glm::vec3 color;
  Vertex2dColor(glm::vec2 position, glm::vec3 color);
};

class Vertex2dUV {
public:
  glm::vec2 position;
  glm::vec2 uv;
  Vertex2dUV(glm::vec2 position, glm::vec2 uv);
};

class Vertex3dColor {
public:
  const glm::vec3 position;
  const glm::vec3 color;
  Vertex3dColor(glm::vec3 position, glm::vec3 color);
};

class Vertex3dUV {
public:
  glm::vec3 position;
  glm::vec2 uv;
  Vertex3dUV(glm::vec3 position, glm::vec2 uv);
};

#endif