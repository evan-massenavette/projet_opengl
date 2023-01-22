#include "point_light.hpp"

namespace shader_structs {

PointLight::PointLight(const glm::vec3& color,
                       const float intensityFactor,
                       const glm::vec3& position,
                       const float attenuationFactor,
                       const bool isOn)
    : color(color),
      intensityFactor(intensityFactor),
      position(position),
      attenuationFactor(attenuationFactor),
      isOn(isOn) {}

GLsizeiptr PointLight::getDataSizeStd140() {
  // Explaination of size :
  // - color + intensityFactor make a first vec4
  // - position + attenuationFactor make second vec4
  // - isOn gets rounded to a third vec4
  return sizeof(glm::vec4) * 3;
}

void* PointLight::getDataPointer() const {
  return (void*)&color;
}

}  // namespace shader_structs