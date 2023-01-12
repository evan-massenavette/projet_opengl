#ifndef SHADER_HPP
#define SHADER_HPP

class Shaders {

private:
  Shaders() = delete;
  Shaders(const Shaders &) = delete;

public:
  static GLuint createProgram(const char *vertexShaderPath,
                              const char *fragmentShaderPath);

private:
  static GLuint _loadOne(const char *filepath, GLenum shaderType,
                         GLuint program);
};

#endif