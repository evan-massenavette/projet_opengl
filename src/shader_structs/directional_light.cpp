#include <iostream>

#include "directional_light.hpp"

namespace shader_structs {

DirectionalLight::DirectionalLight(const glm::vec3& color,
                                   const glm::vec3& direction,
                                   const float factor,
                                   const bool isOn)
    : color(color), direction(direction), factor(factor), isOn(isOn) {}

void DirectionalLight::setUniform(ShaderProgram& shaderProgram,
                                  const std::string& uniformName) const {
  shaderProgram[constructAttributeName(uniformName, "color")] = color;
  shaderProgram[constructAttributeName(uniformName, "direction")] = direction;
  shaderProgram[constructAttributeName(uniformName, "factor")] = factor;
  shaderProgram[constructAttributeName(uniformName, "isOn")] = isOn;
}

GLsizeiptr DirectionalLight::getDataSizeStd140() {
  // Here it's three times size of vec4 because:
  // - position + dummy padding make first vec4
  // - color + factor factor make second vec4
  // - isOn gets rounded to a third vec4
  return sizeof(glm::vec4) * 3;
}

void* DirectionalLight::getDataPointer() const {
  return (void*)&direction;
}

const DirectionalLight& DirectionalLight::none() {
  static DirectionalLight noneDirectionalLight(
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, false);
  return noneDirectionalLight;
}

}  // namespace shader_structs
