#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include "shader_struct.hpp"

namespace shader_structs {

/**
 * Represents directional light in a shader.
 */
struct DirectionalLight : ShaderStruct {
  DirectionalLight(const glm::vec3& direction,
                   const glm::vec3& color,
                   const float factor,
                   const bool isOn = true);

  /**
   * Sets directional light in a shader program.
   */
  void setUniform(ShaderProgram& shaderProgram,
                  const std::string& uniformName) const override;

  /**
   * Gets data size of the structure (in bytes) according to std140 layout
   * rules.
   */
  static GLsizeiptr getDataSizeStd140();
  void* getDataPointer() const override;

  /**
   * Returns a directional light that is off and does not contribute at all.
   */
  static const DirectionalLight& none();

  glm::vec3 direction;       // Direction of the directional light
  float __DUMMY_PADDING0__;  // Needed because of std140 layout padding rules

  glm::vec3 color;  // Color of the directional light
  float factor;     // Factor to multiply dot product with (strength of light)
  GLint isOn;       // Flag telling, if the light is on
};

}  // namespace shader_structs

#endif