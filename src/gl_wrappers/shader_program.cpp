#include <iostream>

#include "shader_program.hpp"

ShaderProgram::~ShaderProgram() {
  deleteProgram();
}

void ShaderProgram::createProgram() {
  _shaderProgramID = glCreateProgram();
}

bool ShaderProgram::addShaderToProgram(const Shader& shader) const {
  if (!shader.isCompiled())
    return false;

  glAttachShader(_shaderProgramID, shader.getShaderID());
  return true;
}

bool ShaderProgram::linkProgram() {
  if (_isLinked) {
    return true;
  }

  // Try to link program and get the link status
  glLinkProgram(_shaderProgramID);
  GLint linkStatus;
  glGetProgramiv(_shaderProgramID, GL_LINK_STATUS, &linkStatus);
  _isLinked = linkStatus == GL_TRUE;

  if (!_isLinked) {
    std::cerr << "Unable to link shader program.";

    // Get length of the error log first
    GLint logLength;
    glGetProgramiv(_shaderProgramID, GL_INFO_LOG_LENGTH, &logLength);

    // If there is some log, then retrieve it and output extra information
    if (logLength > 0) {
      GLchar* logMessage = new GLchar[logLength];
      glGetProgramInfoLog(_shaderProgramID, logLength, nullptr, logMessage);
      std::cerr << "The linker returned: \n" << logMessage;
      delete[] logMessage;
    }

    std::cerr << "\n";
    return false;
  }

  return _isLinked;
}

void ShaderProgram::useProgram() const {
  if (_isLinked) {
    glUseProgram(_shaderProgramID);
  }
}

void ShaderProgram::deleteProgram() {
  if (_shaderProgramID == 0) {
    return;
  }

  std::cout << "Deleting shader program (ID: " << _shaderProgramID << ")\n";
  glDeleteProgram(_shaderProgramID);
  _isLinked = false;
}

GLuint ShaderProgram::getShaderProgramID() const {
  return _shaderProgramID;
}

Uniform& ShaderProgram::operator[](const std::string& varName) {
  if (_uniforms.count(varName) == 0) {
    _uniforms[varName] = Uniform(varName, this);
  }

  return _uniforms[varName];
}

// Model and normal matrix setting is pretty common, that's why this convenience
// function

void ShaderProgram::setModelAndNormalMatrix(const glm::mat4& modelMatrix) {
  (*this)[ShaderConstants::modelMatrix()] = modelMatrix;
  (*this)[ShaderConstants::normalMatrix()] =
      glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
}

GLuint ShaderProgram::getUniformBlockIndex(
    const std::string& uniformBlockName) const {
  if (!_isLinked) {
    std::cerr << "Unable to get index of uniform block '" << uniformBlockName
              << "' because the program isn't linked \n";
    return GL_INVALID_INDEX;
  }

  const auto result =
      glGetUniformBlockIndex(_shaderProgramID, uniformBlockName.c_str());
  if (result == GL_INVALID_INDEX) {
    std::cerr << "Faield to get index of uniform block '" << uniformBlockName
              << "' because no such uniform block exists.\n";
  }

  return result;
}

void ShaderProgram::bindUniformBlockToBindingPoint(
    const std::string& uniformBlockName,
    const GLuint bindingPoint) const {
  const auto blockIndex = getUniformBlockIndex(uniformBlockName);
  if (blockIndex != GL_INVALID_INDEX) {
    glUniformBlockBinding(_shaderProgramID, blockIndex, bindingPoint);
  }
}

void ShaderProgram::setTransformFeedbackRecordedVariables(
    const std::vector<std::string>& recordedVariablesNames,
    const GLenum bufferMode) const {
  std::vector<const char*> recordedVariablesNamesPtrs;
  for (const auto& recordedVariableName : recordedVariablesNames) {
    recordedVariablesNamesPtrs.push_back(recordedVariableName.c_str());
  }

  glTransformFeedbackVaryings(
      _shaderProgramID, static_cast<GLsizei>(recordedVariablesNamesPtrs.size()),
      recordedVariablesNamesPtrs.data(), bufferMode);
}
