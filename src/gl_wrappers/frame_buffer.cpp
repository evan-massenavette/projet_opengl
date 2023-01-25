#include <iostream>

#include "frame_buffer.hpp"

bool FrameBuffer::create(GLsizei width, GLsizei height) {
  if (_frameBufferID != 0) {
    return false;
  }

  // Create the framebuffer object (FBO)
  glGenFramebuffers(1, &_frameBufferID);
  if (_frameBufferID == 0) {
    std::cerr << "Unable to create framebuffer\n";
    return false;
  }

  // Cache attributes into member vars
  _width = width;
  _height = height;

  std::cout << "Created framebuffer (ID: " << _frameBufferID
            << ", dimensions: " << _width << " x " << _height << ")\n";

  return true;
}

bool FrameBuffer::addTexture(GLenum internalFormat,
                             GLenum attachment,
                             GLenum textureUnit) {
  if (_frameBufferID == 0) {
    return false;
  }

  // Create an empty texture with no data and same size as framebuffer
  _texture = std::make_unique<Texture>();
  _texture->create(_width, _height, internalFormat);

  // Attach it to the framebuffer
  _texture->bind(textureUnit);
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D,
                         _texture->getID(), 0);
  return true;
}

bool FrameBuffer::addTextureCubeMap(GLenum internalFormat,
                                    GLenum attachment,
                                    GLenum textureUnit) {
  if (_frameBufferID == 0) {
    return false;
  }

  // Create a texture cube map of same size as framebuffer
  _textureCubeMap = std::make_unique<TextureCubeMap>();
  _textureCubeMap->create(_width, _height, GL_FLOAT);

  // Attach it to the framebuffer
  _textureCubeMap->bind(textureUnit);
  glFramebufferTexture(GL_FRAMEBUFFER, attachment, _textureCubeMap->getID(), 0);
  return true;
}

bool FrameBuffer::resize(GLsizei newWidth, GLsizei newHeight) {
  if (_frameBufferID == 0) {
    return false;
  }

  deleteOnlyFrameBuffer();
  // First create and bind framebuffer object (FBO)
  glGenFramebuffers(1, &_frameBufferID);
  if (_frameBufferID == 0) {
    std::cerr << "Unable to create framebuffer during resizing.\n";
    return false;
  }

  std::cout << "Resizing framebuffer (ID: " << _frameBufferID
            << ", new dimensions: " << newWidth << " x " << newHeight << ")\n";

  if (_texture) {
    if (!_texture->resize(newWidth, newHeight)) {
      std::cerr
          << "Unable to resize texture attachment for the framebuffer (ID: "
          << _frameBufferID << ")\n";
      deleteFrameBuffer();
      return false;
    }

    _texture->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           _texture->getID(), 0);
  }

  if (_textureCubeMap) {
    if (!_textureCubeMap->resize(newWidth, newHeight)) {
      std::cerr << "Unable to resize texture cube map attachment for the "
                   "framebuffer (ID: "
                << _frameBufferID << ")\n";
      deleteFrameBuffer();
      return false;
    }

    _textureCubeMap->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           _textureCubeMap->getID(), 0);
  }

  // Check FBO status when all attachments have been attached
  const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
    return false;
  }

  _width = newWidth;
  _height = newHeight;

  return true;
}

void FrameBuffer::setFullViewport() const {
  glViewport(0, 0, _width, _height);
}

GLsizei FrameBuffer::getWidth() const {
  return _width;
}

GLsizei FrameBuffer::getHeight() const {
  return _height;
}

Texture* FrameBuffer::getTexture() const {
  return _texture.get();
}

TextureCubeMap* FrameBuffer::getTextureCubeMap() const {
  return _textureCubeMap.get();
}

bool FrameBuffer::isComplete() const {
  if (_frameBufferID == 0) {
    return false;
  }

  // Check FBO status when all attachments have been attached
  const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
    return false;
  }

  return true;
}

void FrameBuffer::bindAsReadAndDraw() const {
  glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferID);
}

void FrameBuffer::bindAsRead() const {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBufferID);
}

void FrameBuffer::bindAsDraw() const {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBufferID);
}

void FrameBuffer::unbindAsReadAndDraw() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::unbindAsRead() const {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBuffer::unbindAsDraw() const {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
  deleteFrameBuffer();
}

void FrameBuffer::deleteFrameBuffer() {
  _texture.reset();
  _textureCubeMap.reset();
  deleteOnlyFrameBuffer();
}

void FrameBuffer::deleteOnlyFrameBuffer() {
  if (_frameBufferID == 0) {
    return;
  }

  std::cout << "Deleting framebuffer (ID: " << _frameBufferID << ")\n";
  glDeleteFramebuffers(1, &_frameBufferID);
  _frameBufferID = 0;
  _width = 0;
  _height = 0;
}

void FrameBuffer::Default::bindAsReadAndDraw() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Default::bindAsRead() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBuffer::Default::bindAsDraw() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

GLint FrameBuffer::Default::getDepthBits() {
  GLint depthBits = -1;
  bindAsRead();
  glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, GL_DEPTH,
                                        GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
                                        &depthBits);
  return depthBits;
}

GLint FrameBuffer::Default::getStencilBits() {
  GLint stencilBits = -1;
  bindAsRead();
  glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, GL_STENCIL,
                                        GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
                                        &stencilBits);
  return stencilBits;
}

void FrameBuffer::Default::setFullViewport(const App& app) {
  const auto windowWidth = app.getWindowWidth();
  const auto windowHeight = app.getWindowHeight();

  glViewport(0, 0, windowWidth, windowHeight);
}
