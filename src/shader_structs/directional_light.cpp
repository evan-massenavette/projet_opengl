#include <iostream>

#include "directional_light.hpp"

namespace shader_structs {

DirectionalLight::DirectionalLight(const glm::vec3& color,
                                   const glm::vec3& direction,
                                   const float intensityFactor,
                                   const bool isOn)
    : color(color),
      direction(glm::normalize(direction)),
      intensityFactor(intensityFactor),
      isOn(isOn) {}

GLsizeiptr DirectionalLight::getDataSizeStd140() {
  // Explaination of size :
  // - direction + dummy padding make first vec4
  // - color + factor factor make second vec4
  // - isOn gets rounded to a third vec4
  return sizeof(glm::vec4) * 3;
}

void* DirectionalLight::getDataPointer() const {
  return (void*)&direction;
}

}  // namespace shader_structs
