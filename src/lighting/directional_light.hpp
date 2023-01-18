#ifndef LIGHTSOURCE_HPP
#define LIGHTSOURCE_HPP

#include <glm/glm.hpp>

class DirectionalLight {
public:
  glm::vec3 direction;
  glm::vec3 color;

  DirectionalLight(glm::vec3 direction, glm::vec3 color);
};
#endif