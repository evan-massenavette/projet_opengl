#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include <glad/glad.h>

#include "shader_struct.hpp"

namespace shader_structs {

/**
 * Represents point light in a shader.
 */
struct PointLight : ShaderStruct {
  PointLight(const glm::vec3& color,
             const float intensityFactor,
             const glm::vec3& position,
             const float attenuationFactor,
             const bool isOn = true);

  /**
   * Gets data size of the structure (in bytes) according to std140 layout
   * rules.
   */
  static GLsizeiptr getDataSizeStd140();
  void* getDataPointer() const override;

  glm::vec3 color;          // Color of the point light
  float intensityFactor;    // Strength of light
  glm::vec3 position;       // Position of the point light
  float attenuationFactor;  // Speed of attenuation with distance
  GLint isOn;               // Flag telling, if the light is on
};

}  // namespace shader_structs

#endif