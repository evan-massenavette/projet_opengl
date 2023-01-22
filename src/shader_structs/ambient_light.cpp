#include "ambient_light.hpp"

namespace shader_structs {

AmbientLight::AmbientLight(const glm::vec3& color,
                           const float intensityFactor,
                           const bool isOn)
    : color(color), intensityFactor(intensityFactor), isOn(isOn) {}

GLsizeiptr AmbientLight::getDataSizeStd140() {
  // Explaination of size :
  // - color + intensityFactor make a first vec4
  // - isOn gets rounded to a second vec4
  return sizeof(glm::vec4) * 2;
}

void* AmbientLight::getDataPointer() const {
  return (void*)&color;
}

glm::vec3 AmbientLight::getColorContribution() const {
  return isOn ? color : glm::vec3(0.0f);
}

}  // namespace shader_structs