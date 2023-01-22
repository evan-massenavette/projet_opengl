#include <iostream>

#include "directional_light.hpp"

namespace shader_structs {

DirectionalLight::DirectionalLight(const glm::vec3& color,
                                   const float intensityFactor,
                                   const glm::vec3& direction,
                                   const bool isOn)
    : color(color),
      intensityFactor(intensityFactor),
      direction(glm::normalize(direction)),
      isOn(isOn) {}

GLsizeiptr DirectionalLight::getDataSizeStd140() {
  // Explaination of size :
  // - color + intensityFactor padding make a first vec4
  // - direction + isOn factor make a second vec4
  return sizeof(glm::vec4) * 2;
}

void* DirectionalLight::getDataPointer() const {
  return (void*)&color;
}

}  // namespace shader_structs
