#include "material.hpp"

namespace shader_structs {

Material::Material(const glm::vec3& ambient,
                   const glm::vec3& diffuse,
                   const glm::vec3& specular,
                   const float shininess)
    : ambient(ambient),
      diffuse(diffuse),
      specular(specular),
      shininess(shininess) {}

const Material& Material::default() {
  static Material defaultMaterial(glm::vec3(1), glm::vec3(1), glm::vec3(1), 50);
  return defaultMaterial;
}

void Material::setUniform(ShaderProgram& shaderProgram,
                          const std::string& uniformName) const {
  shaderProgram[getAttributeName(uniformName, "ambient")] = ambient;
  shaderProgram[getAttributeName(uniformName, "diffuse")] = diffuse;
  shaderProgram[getAttributeName(uniformName, "specular")] = specular;
  shaderProgram[getAttributeName(uniformName, "shininess")] = shininess;
}

GLsizeiptr Material::getDataSizeStd140() {
  // Explaination of size :
  // - ambient + dummy padding make first vec4
  // - diffuse + dummy padding make second vec4
  // - specular + shininess make third vec4
  return sizeof(glm::vec4) * 3;
}

void* Material::getDataPointer() const {
  return (void*)&ambient;
}

}  // namespace shader_structs