#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "shaders.hpp"

GLuint Shaders::_loadOne(const char *filepath, GLenum shaderType,
                         GLuint program) {

  printf("Loading shader: %s\n", filepath);

  // Read the shader code from file
  std::string shaderCode;
  std::ifstream shaderStream(filepath);
  if (shaderStream.is_open()) {
    std::stringstream sstr;
    sstr << shaderStream.rdbuf();
    shaderCode = sstr.str();
    shaderStream.close();
  } else {
    printf("Can't open shader: %s", filepath);
    getchar();
    exit(EXIT_FAILURE);
  }

  GLint result = GL_FALSE;
  int infoLogLength;

  // Create the shader
  GLuint shader = glCreateShader(shaderType);

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

GLuint Shaders::createProgram(const char *vertexShaderPath,
                              const char *fragmentShaderPath) {

  // Create the program
  GLuint program = glCreateProgram();

  // Load both shaders
  GLuint vertexShader =
      Shaders::_loadOne(vertexShaderPath, GL_VERTEX_SHADER, program);
  GLuint fragmentShader =
      Shaders::_loadOne(fragmentShaderPath, GL_FRAGMENT_SHADER, program);

  // Link the program
  glLinkProgram(program);

  // Cleanup program and shaders
  glDetachShader(program, vertexShader);
  glDeleteShader(vertexShader);
  glDetachShader(program, fragmentShader);
  glDeleteShader(fragmentShader);

  return program;
}