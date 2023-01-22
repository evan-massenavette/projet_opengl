#include <iostream>

#include "shader_program.hpp"
#include "uniform.hpp"

Uniform::Uniform(const std::string &name, ShaderProgram *shaderProgram)
    : _name(name), _shaderProgram(shaderProgram) {
  _location =
      glGetUniformLocation(_shaderProgram->getShaderProgramID(), _name.c_str());
  if (_location == -1) {
    std::cout << "WARNING: uniform with name " << name
              << " does not exist, setting it will fail!" << std::endl;
  }
}

Uniform &Uniform::operator=(const glm::vec2 &vector2D) {
  set(vector2D);
  return *this;
}

// Family of functions setting vec2 uniforms

void Uniform::set(const glm::vec2 &vector2D) const {
  glUniform2fv(_location, 1, reinterpret_cast<const GLfloat *>(&vector2D));
}

void Uniform::set(const glm::vec2 *vectors2D, GLsizei count) const {
  glUniform2fv(_location, count, reinterpret_cast<const GLfloat *>(vectors2D));
}

// Family of functions setting vec3 uniforms

Uniform &Uniform::operator=(const glm::vec3 &vector3D) {
  set(vector3D);
  return *this;
}

void Uniform::set(const glm::vec3 &vector3D) const {
  glUniform3fv(_location, 1, reinterpret_cast<const GLfloat *>(&vector3D));
}

void Uniform::set(const glm::vec3 *vectors3D, GLsizei count) const {
  glUniform3fv(_location, count, reinterpret_cast<const GLfloat *>(vectors3D));
}

// Family of functions setting vec4 uniforms

Uniform &Uniform::operator=(const glm::vec4 &vector4D) {
  set(vector4D);
  return *this;
}

void Uniform::set(const glm::vec4 &vector4D) const {
  glUniform4fv(_location, 1, reinterpret_cast<const GLfloat *>(&vector4D));
}

void Uniform::set(const glm::vec4 *vectors4D, GLsizei count) const {
  glUniform4fv(_location, count, reinterpret_cast<const GLfloat *>(vectors4D));
}

// Family of functions setting float uniforms

Uniform &Uniform::operator=(const GLfloat floatValue) {
  set(floatValue);
  return *this;
}

Uniform &Uniform::operator=(const std::vector<GLfloat> &floatValues) {
  set(floatValues.data(), static_cast<GLsizei>(floatValues.size()));
  return *this;
}

void Uniform::set(GLfloat floatValue) const {
  glUniform1fv(_location, 1, static_cast<const GLfloat *>(&floatValue));
}

void Uniform::set(const GLfloat *floatValues, GLsizei count) const {
  glUniform1fv(_location, count, floatValues);
}

// Family of functions setting integer uniforms

Uniform &Uniform::operator=(const GLint integerValue) {
  set(integerValue);
  return *this;
}

void Uniform::set(GLint integerValue) const {
  glUniform1iv(_location, 1, static_cast<const GLint *>(&integerValue));
}

void Uniform::set(const GLint *integerValues, GLsizei count) const {
  glUniform1iv(_location, count, integerValues);
}

// Family of functions setting 3x3 matrices uniforms

Uniform &Uniform::operator=(const glm::mat3 &matrix) {
  set(matrix);
  return *this;
}

void Uniform::set(const glm::mat3 &matrix) const {
  glUniformMatrix3fv(_location, 1, false,
                     reinterpret_cast<const GLfloat *>(&matrix));
}

void Uniform::set(const glm::mat3 *matrices, GLsizei count) const {
  glUniformMatrix3fv(_location, count, false,
                     reinterpret_cast<const GLfloat *>(matrices));
}

// Family of functions setting 4x4 matrices uniforms

Uniform &Uniform::operator=(const glm::mat4 &matrix) {
  set(matrix);
  return *this;
}

void Uniform::set(const glm::mat4 &matrix) const {
  glUniformMatrix4fv(_location, 1, false,
                     reinterpret_cast<const GLfloat *>(&matrix));
}

void Uniform::set(const glm::mat4 *matrices, GLsizei count) const {
  glUniformMatrix4fv(_location, count, false,
                     reinterpret_cast<const GLfloat *>(matrices));
}
