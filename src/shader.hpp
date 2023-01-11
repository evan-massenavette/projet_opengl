#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
private:
  const char *_filepath;
  const GLenum _shaderType;

  GLuint _load(GLuint program);

public:
  Shader(const char *filepath, GLenum shaderType);
  static void loadAll(GLuint program, std::vector<Shader> &shaders);
};

#endif