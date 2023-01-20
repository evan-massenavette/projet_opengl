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
                   const float intensityFactor,
                   const bool isOn = true);

  /**
   * Gets data size of the structure (in bytes) according to std140 layout
   * rules.
   */
  static GLsizeiptr getDataSizeStd140();
  void* getDataPointer() const override;

  glm::vec3 direction;       // Direction of the directional light
  float __DUMMY_PADDING0__;  // Needed because of std140 layout padding rules

  glm::vec3 color;        // Color of the directional light
  float intensityFactor;  // Strength of light
  GLint isOn;             // Flag telling, if the light is on
};

}  // namespace shader_structs

#endif