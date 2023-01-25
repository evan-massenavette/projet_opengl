#include <iostream>
#include <mutex>

#include "texture_cube_map.hpp"

TextureCubeMap::~TextureCubeMap() {
  std::cout << "Deleting texture cube map (ID: " << _textureID << ")\n";
  deleteTexture();
}

bool TextureCubeMap::create(GLsizei width, GLsizei height, GLenum format) {
  if (isLoaded()) {
    return false;
  }

  // Update info
  _width = width;
  _height = height;
  _format = format;

  // Create the texture
  glGenTextures(1, &_textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
  for (GLuint i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, _format, _width,
                 _height, 0, _format, GL_FLOAT, nullptr);
  }

  // Filtering
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Wrapping
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  std::cout << "Created texture cube map (ID: " << _textureID << ")\n";

  return true;
}

void TextureCubeMap::bind(const GLenum textureUnit) const {
  if (!_isLoadedLogged()) {
    return;
  }

  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _textureID);
}

void TextureCubeMap::unbind(const GLenum textureUnit) const {
  if (!_isLoadedLogged()) {
    return;
  }

  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubeMap::deleteTexture() {
  if (!isLoaded()) {
    return;
  }

  glDeleteTextures(1, &_textureID);
  _textureID = 0;
  _width = 0;
  _height = 0;
  _format = 0;
}

GLuint TextureCubeMap::getID() const {
  return _textureID;
}

int TextureCubeMap::getWidth() const {
  return _width;
}

int TextureCubeMap::getHeight() const {
  return _height;
}

bool TextureCubeMap::isLoaded() const {
  return _textureID != 0;
}

bool TextureCubeMap::resize(GLsizei newWidth, GLsizei newHeight) {
  if (!_isLoadedLogged()) {
    return false;
  }

  const auto oldFormat = _format;
  deleteTexture();

  return create(newWidth, newHeight, oldFormat);
}

bool TextureCubeMap::_isLoadedLogged() const {
  if (!isLoaded()) {
    std::cout << "Attempting to access non-loaded texture\n";
    return false;
  }

  return true;
}
