#ifndef SHADER_STRUCT_HPP
#define SHADER_STRUCT_HPP

#include "../gl_wrappers/shader_program.hpp"

namespace shader_structs {

/**
 * Base class for uniform shader structures in shader program.
 */
struct ShaderStruct {
 public:
  /**
   * Sets this shader structure as uniform variable.
   *
   * @param shaderProgram  Shader program to set uniform variable in
   * @param uniformName    Name of the uniform variable
   */
  virtual void setUniform(ShaderProgram& shaderProgram,
                          const std::string& uniformName) const {}

  virtual void* getDataPointer() const { return nullptr; }

 protected:
  /**
   * Constructs full uniform name containing of structure name and attribute
   * name.
   *
   * @param uniformName    Name of the uniform variable
   * @param attributeName  Name of the attribute in that structure
   *
   * @return Full uniform name.
   */
  static std::string getAttributeName(const std::string& uniformName,
                                      const std::string& attributeName);
};

}  // namespace shader_structs

#endif