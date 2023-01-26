#include <iostream>
#include <mutex>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.hpp"

Texture::~Texture() {
  std::cout << "Deleting texture (ID: " << _textureID << ")\n";
  deleteTexture();
}

bool Texture::createFromData(const unsigned char* data,
                             GLsizei width,
                             GLsizei height,
                             GLenum format,
                             bool generateMipmaps) {
  if (isLoaded()) {
    return false;
  }

  // Update info
  _width = width;
  _height = height;
  _format = format;

  // Create the texture
  glGenTextures(1, &_textureID);
  glBindTexture(GL_TEXTURE_2D, _textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format,
               GL_UNSIGNED_BYTE, data);

  // Set the texture's parameters
  _setParameters();

  if (generateMipmaps) {
    // Generate mipmaps automatically
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  std::cout << "Created texture (ID: " << _textureID << ")\n";

  return true;
}

bool Texture::create(GLsizei width, GLsizei height, GLenum format) {
  if (isLoaded()) {
    return false;
  }

  // Update info
  _width = width;
  _height = height;
  _format = format;

  // Create the texture
  glGenTextures(1, &_textureID);
  glBindTexture(GL_TEXTURE_2D, _textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format,
               GL_UNSIGNED_BYTE, nullptr);

  // Set the texture's parameters
  _setParameters();

  std::cout << "Created texture (ID: " << _textureID << ")\n";

  return true;
}

bool Texture::loadTexture2D(const std::string& filePath, bool generateMipmaps) {
  stbi_set_flip_vertically_on_load(1);
  int bytesPerPixel;
  const auto imageData =
      stbi_load(filePath.c_str(), &_width, &_height, &bytesPerPixel, 0);
  if (imageData == nullptr) {
    std::cerr << "Unable to load texture image: " << filePath << "\n";
    return false;
  }

  GLenum format = 0;
  if (bytesPerPixel == 4) {
    format = GL_RGBA;
  } else if (bytesPerPixel == 3) {
    format = GL_RGB;
  } else if (bytesPerPixel == 1) {
    format = GL_DEPTH_COMPONENT;
  }

  const auto result =
      createFromData(imageData, _width, _height, format, generateMipmaps);
  stbi_image_free(imageData);
  _filePath = filePath;
  return result;
}

void Texture::bind(const GLenum textureUnit) const {
  if (!isLoadedLogged()) {
    return;
  }

  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::unbind(const GLenum textureUnit) const {
  if (!isLoadedLogged()) {
    return;
  }

  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deleteTexture() {
  if (!isLoaded()) {
    return;
  }

  glDeleteTextures(1, &_textureID);
  _textureID = 0;
  _width = _height = 0;
  _format = 0;
}

GLuint Texture::getID() const {
  return _textureID;
}

std::string Texture::getFilePath() const {
  return _filePath;
}

int Texture::getWidth() const {
  return _width;
}

int Texture::getHeight() const {
  return _height;
}

bool Texture::isLoaded() const {
  return _textureID != 0;
}

bool Texture::resize(GLsizei newWidth, GLsizei newHeight) {
  if (!isLoadedLogged()) {
    return false;
  }

  const auto oldFormat = _format;
  deleteTexture();

  return createFromData(nullptr, newWidth, newHeight, oldFormat, false);
}

int Texture::getNumTextureImageUnits() {
  static std::once_flag queryOnceFlag;
  static int maxTextureUnits;
  std::call_once(queryOnceFlag, []() {
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
  });

  return maxTextureUnits;
}

bool Texture::isLoadedLogged() const {
  if (!isLoaded()) {
    std::cout << "Attempting to access non-loaded texture\n";
    return false;
  }

  return true;
}

void Texture::_setParameters() const {
  // Filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  // Wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

std::shared_ptr<Texture> Texture::getMissingTexture() {
  static std::shared_ptr<Texture> missingTexture;
  if (!missingTexture) {
    missingTexture = std::make_shared<Texture>();
    missingTexture->loadTexture2D("models/missing_texture.bmp");
  }
  return missingTexture;
}