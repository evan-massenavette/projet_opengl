#ifndef AMBIENT_LIGHT_HPP
#define AMBIENT_LIGHT_HPP

#include "shader_struct.hpp"

namespace shader_structs {

/**
 * Represents ambient light in a shader.
 */
struct AmbientLight : ShaderStruct {
  AmbientLight(const glm::vec3& color,
               const float intensityFactor,
               const bool isOn = true);

  /**
   * Gets data size of the structure (in bytes) according to std140 layout
   * rules.
   */
  static GLsizeiptr getDataSizeStd140();
  void* getDataPointer() const override;

  /**
   * Gets the final contributed color of this ambient light (depending if light
   * is on or off). The ambient light color contribution.
   */
  glm::vec3 getColorContribution() const;

  glm::vec3 color;        // Color of the ambient light
  float intensityFactor;  // Strength of light
  GLint isOn;             // Flag telling, if the light is on
};

}  // namespace shader_structs

#endif