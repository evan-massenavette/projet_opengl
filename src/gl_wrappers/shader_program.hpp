#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <map>
#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "shader.hpp"
#include "uniform.hpp"

/**
 * Wraps OpenGL shader program (creation, adding shaders, linking)
 */
class ShaderProgram {
 public:
  ~ShaderProgram();

  /**
   * Creates a new shader program.
   */
  void createProgram();

  /**
   * Adds a shader to shader program.
   * The given shader must already be loaded and compiled.
   * @return True, if the shader has been added or false otherwise.
   */
  bool addShaderToProgram(const Shader& shader) const;

  /**
   * Links the program.
   * If the function succeeds, shader program is ready to use.
   * @return True if the shader has been linked, or false otherwise.
   */
  bool linkProgram();

  /**
   * Uses this shader program.
   */
  void useProgram() const;

  /**
   * Deletes this shader program.
   */
  void deleteProgram();

  /**
   * Gets OpenGL-assigned shader program ID.
   */
  GLuint getShaderProgramID() const;

  /**
   * Gets the uniform variable of the name.
   * Creates a new one on first access, otherwise it's retrieved from cache.
   * @param varName Name of the uniform variable
   * @return Uniform variable (even if it does not exist: it will be invalid)
   */
  Uniform& operator[](const std::string& varName);

  /**
   * Sets the model and normal matrix at once.
   * @param modelMatrix  Model matrix to be set, used to calculate the normal
   * matrix
   */
  void setModelAndNormalMatrix(const glm::mat4& modelMatrix);

  /**
   * Gets index of given uniform block in this shader program.
   * @param uniformBlockName Name of the uniform block
   * @return Index of uniform block or GL_INVALID_INDEX if such a block doesn't
   * exist or if some other error occurs
   */
  GLuint getUniformBlockIndex(const std::string& uniformBlockName) const;

  /**
   * Binds the uniform block of this program to a uniform binding point.
   * @param uniformBlockName  Name of the uniform block
   * @param bindingPoint      Uniform binding point to bind the uniform block to
   *
   * @see
   * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUniformBlockBinding.xhtml
   */
  void bindUniformBlockToBindingPoint(const std::string& uniformBlockName,
                                      GLuint bindingPoint) const;

  /**
   * Tells OpenGL, which output variables should be recorded during transform
   * feedback. From early OpenGL versions (3.0), a term varying was used for
   * variables shared between shader stages, but this name is deprecated for
   * quite some time. But for legacy reasons, OpenGL function names still use
   * the term varying.
   * @param recordedVariablesNames  Names of the output variables to be recorded
   * @param bufferMode              Buffer mode to use (usually
   * GL_INTERLEAVED_ATTRIBS)
   */
  void setTransformFeedbackRecordedVariables(
      const std::vector<std::string>& recordedVariablesNames,
      GLenum bufferMode = GL_INTERLEAVED_ATTRIBS) const;

 private:
  GLuint _shaderProgramID = 0;  // OpenGL-assigned shader program ID
  bool _isLinked = false;       // Flag telling whether shader program has been
                                // successfully linked
  std::map<std::string, Uniform>
      _uniforms;  // Cache of uniform locations (to reduces OpenGL calls)
};

/**
 * Storage for the keys of commonly used shader programs
 */
class ShaderProgramKeys {
 public:
  DEFINE_SHADER_CONSTANT(main, "main");
  DEFINE_SHADER_CONSTANT(depth, "depth");
};

#endif