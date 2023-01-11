#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "shader.hpp"

Shader::Shader(const char *filepath, GLenum shaderType)
    : _filepath(filepath), _shaderType(shaderType) {}

GLuint Shader::_load(GLuint program) {

  printf("Loading shader: %s\n", _filepath);

  // Read the shader code from file
  std::string shaderCode;
  std::ifstream shaderStream(_filepath);
  if (shaderStream.is_open()) {
    std::stringstream sstr;
    sstr << shaderStream.rdbuf();
    shaderCode = sstr.str();
    shaderStream.close();
  } else {
    printf("Can't open shader: %s", _filepath);
    getchar();
    exit(EXIT_FAILURE);
  }

  GLint result = GL_FALSE;
  int infoLogLength;

  // Create the shader
  GLuint shader = glCreateShader(_shaderType);

  // Compile the shader
  char const *shaderCodePtr = shaderCode.c_str();
  glShaderSource(shader, 1, &shaderCodePtr, NULL);
  glCompileShader(shader);

  // Check the shader for error
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
  if (infoLogLength > 0) {
    std::vector<char> shaderErrorMessage(infoLogLength + 1);
    glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderErrorMessage[0]);
    printf("%s\n", &shaderErrorMessage[0]);
    exit(EXIT_FAILURE);
  }

  // Link the program
  glAttachShader(program, shader);

  // Check the program for error
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
  if (infoLogLength > 0) {
    std::vector<char> programErrorMessage(infoLogLength + 1);
    glGetProgramInfoLog(program, infoLogLength, NULL, &programErrorMessage[0]);
    printf("%s\n", &programErrorMessage[0]);
    exit(EXIT_FAILURE);
  }

  return shader;
}

void Shader::loadAll(GLuint program, std::vector<Shader> &shaders) {

  // Load all shaders and get their IDs
  std::vector<GLuint> shaderIds(shaders.size());
  for (auto &shader : shaders) {
    shaderIds.push_back(shader._load(program));
  }

  // Link the program
  glLinkProgram(program);

  // Cleanup program and shaders
  for (const auto &shaderId : shaderIds) {
    glDetachShader(program, shaderId);
    glDeleteShader(shaderId);
  }
}