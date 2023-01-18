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

#endif