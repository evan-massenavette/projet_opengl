#include "fog_parameters.hpp"

namespace shader_structs {

FogParameters::FogParameters(const glm::vec3& color,
                             float density,
                             const bool isEnabled)
    : color(color), density(density), isEnabled(isEnabled) {}

const FogParameters& FogParameters::noFog() {
  static FogParameters noFogParameters(glm::vec3(0), 0, false);
  return noFogParameters;
}

void FogParameters::setUniform(ShaderProgram& shaderProgram,
                               const std::string& uniformName) const {
  shaderProgram[getAttributeName(uniformName, "isEnabled")] = isEnabled;
  if (!isEnabled) {
    return;  // Skip settings other parameters if fog is not enabled
  }
  shaderProgram[getAttributeName(uniformName, "color")] = color;
  shaderProgram[getAttributeName(uniformName, "density")] = density;
}

}  // namespace shader_structs