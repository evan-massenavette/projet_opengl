#ifndef SHADER_HPP
#define SHADER_HPP

#include <set>
#include <string>
#include <vector>

#include <glad/glad.h>

#define DEFINE_SHADER_CONSTANT(constantName, constantValue) \
  static const std::string constantName() {                 \
    static std::string value = constantValue;               \
    return value;                                           \
  }

/**
 * Storage for commonly used constants in shader programs.
 */
class ShaderConstants {
 public:
  // Matrices
  DEFINE_SHADER_CONSTANT(modelMatrix, "matrices.model");
  DEFINE_SHADER_CONSTANT(projectionMatrix, "matrices.projection");
  DEFINE_SHADER_CONSTANT(viewMatrix, "matrices.view");
  DEFINE_SHADER_CONSTANT(normalMatrix, "matrices.normal");

  // Color and textures
  DEFINE_SHADER_CONSTANT(color, "color");
  DEFINE_SHADER_CONSTANT(albedoSampler, "albedoSampler");
  DEFINE_SHADER_CONSTANT(depthSampler, "depthSampler");

  // Lighting
  DEFINE_SHADER_CONSTANT(material, "material");
  DEFINE_SHADER_CONSTANT(cameraWorldPos, "cameraWorldPos");
  DEFINE_SHADER_CONSTANT(farPlane, "farPlane");

  // Fog constants
  DEFINE_SHADER_CONSTANT(fogParams, "fogParams");

  // MD2 Animation
  DEFINE_SHADER_CONSTANT(interpolationFactor, "interpolationFactor")
};

/**
 * Wraps OpenGL shader (loading, compilation)
 */
class Shader {
 public:
  ~Shader();

  /**
   * Loads and compiles shader from a specified file.
   * @param fileName    Path to a shader file
   * @param shaderType  Type of shader (vertex, fragment, geometry)
   * @return True if the shader has been successfully loaded and compiled,
   * false otherwise
   */
  bool loadShaderFromFile(const std::string& fileName, GLenum shaderType);

  /**
   * Checks if shader has been loaded and compiled successfully.
   * @return True if the shader has been successfully loaded and compiled,
   * false otherwise
   */
  bool isCompiled() const;

  /**
   * Deletes shader object from OpenGL.
   */
  void deleteShader();

  /**
   * Gets OpenGL-assigned shader ID.
   */
  GLuint getShaderID() const;

  /**
   * Gets shader type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, ...).
   */
  GLenum getShaderType() const;

 private:
  /**
   * Gets all lines from specified shader file.
   * @param fileName Name of file to read the lines from.
   * @param result std::vector to put the lines of source code into
   * @param isReadingIncludedFile Flag saying whether we are reading a file
   * that's been included (will only read lines inside #include_part)
   * @return True if the loading has been successful, or false otherwise
   */
  bool getLinesFromFile(const std::string& fileName,
                        std::vector<std::string>& result,
                        std::set<std::string>& filesIncludedAlready,
                        bool isReadingIncludedFile = false) const;

  GLuint _shaderID = 0;      // OpenGL-assigned shader ID
  GLenum _shaderType = 0;    // Type of shader
  bool _isCompiled = false;  // Flag telling, whether shader has been loaded
                             // and compiled successfully
};

#endif
