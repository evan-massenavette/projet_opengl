#include "point_light.hpp"

namespace shader_structs {

PointLight::PointLight(const glm::vec3& position,
                       const glm::vec3& color,
                       const float intensityFactor,
                       const bool isOn)
    : position(position),
      color(color),
      intensityFactor(intensityFactor),
      isOn(isOn) {}

GLsizeiptr PointLight::getDataSizeStd140() {
  // Explaination of size :
  // - position + dummy padding make first vec4
  // - color + factor make second vec4
  // - isOn gets rounded to a third vec4
  return sizeof(glm::vec4) * 3;
}

void* PointLight::getDataPointer() const {
  return (void*)&position;
}

}  // namespace shader_structs