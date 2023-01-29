#ifndef FOG_PARAMETERS_HPP
#define FOG_PARAMETERS_HPP

#include "shader_struct.hpp"

namespace shader_structs {

/**
 * Represents fog parameters in a shader.
 */
struct FogParameters : ShaderStruct {
  FogParameters(const glm::vec3& color,
                float density,
                const bool isEnabled = true);

  /**
   * Fog parameters for using no fog (not enabled fog).
   */
  static const FogParameters& noFog();

  /**
   * Sets fog parameters in a shader program.
   *
   * @param shaderProgram  Shader program to set fog parameters in
   * @param uniformName    Name of the uniform variable
   */
  void setUniform(ShaderProgram& shaderProgram,
                  const std::string& uniformName) const override;

  glm::vec3 color;  // Color of the fog
  float density;    // Density of the fog
  bool isEnabled;   // Whether the fog is enabled
};

}  // namespace shader_structs

#endif