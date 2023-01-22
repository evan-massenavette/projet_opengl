#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "shader_struct.hpp"

namespace shader_structs {

/**
 * Represents material in a shader.
 */
struct Material : ShaderStruct {
  Material(const glm::vec3& ambient,
           const glm::vec3& diffuse,
           const glm::vec3& specular,
           const float shininess);

  /**
   * Material that does no calculations (is not enabled).
   */
  static const Material& defaultOne();

  /**
   * Sets material structure in a shader program.
   *
   * @param shaderProgram  Shader program to set material in
   * @param uniformName    Name of the uniform variable
   */
  void setUniform(ShaderProgram& shaderProgram,
                  const std::string& uniformName) const override;

  /**
   * Gets data size of the structure (in bytes) according to std140 layout
   * rules.
   */
  static GLsizeiptr getDataSizeStd140();
  void* getDataPointer() const override;

  glm::vec3 ambient;  // Color the material reflects under ambient lighting
  float __DUMMY_PADDING0__;  // Needed because of std140 layout padding rules
  glm::vec3 diffuse;  // Color the material reflects under diffuse lighting
  float __DUMMY_PADDING1__;  // Needed because of std140 layout padding rules
  glm::vec3 specular;  // Color the material reflects under specular lighting
  float shininess;  // Impacts the scattering/radius of the specular highlight
};

}  // namespace shader_structs

#endif