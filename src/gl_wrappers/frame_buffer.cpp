#include <iostream>

#include "frame_buffer.hpp"

FrameBuffer::~FrameBuffer() {
  deleteFrameBuffer();
}

bool FrameBuffer::createFrameBufferWithColorAndDepth(const GLsizei width,
                                                     const GLsizei height) {
  if (_frameBufferID != 0) {
    return false;
  }

  // First create and bind framebuffer object (FBO)
  glGenFramebuffers(1, &_frameBufferID);
  if (_frameBufferID == 0) {
    std::cerr << "Unable to create framebuffer\n";
    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferID);
  std::cout << "Created framebuffer (ID: " << _frameBufferID
            << ", dimensions: " << width << " x " << height << ")\n";

  // Create color render buffer and attach it to FBO
  auto colorRenderBuffer = std::make_unique<RenderBuffer>();
  if (!colorRenderBuffer->create(GL_RGBA8, width, height)) {
    std::cerr << "Unable to create color attachment for the framebuffer (ID: "
              << _frameBufferID << ")\n";
    deleteFrameBuffer();
    return false;
  }

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, colorRenderBuffer->getID());

  // Create depth buffer and attach it to FBO
  auto depthRenderBuffer = std::make_unique<RenderBuffer>();
  if (!depthRenderBuffer->create(GL_DEPTH_COMPONENT24, width, height)) {
    std::cerr << "Unable to create depth attachment for the framebuffer (ID: "
              << _frameBufferID << ")\n";
    deleteFrameBuffer();
    return false;
  }

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depthRenderBuffer->getID());

  // Check FBO status when all attachments have been attached
  const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
    return false;
  }

  // When everything went well, cache attributes into member variables
  _colorRenderBuffer = std::move(colorRenderBuffer);
  _depthRenderBuffer = std::move(depthRenderBuffer);
  _width = width;
  _height = height;

  return true;
}

void FrameBuffer::bindAsBothReadAndDraw() const {
  glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferID);
}

void FrameBuffer::bindAsRead() const {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBufferID);
}

void FrameBuffer::bindAsDraw() const {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBufferID);
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

  glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferID);
  std::cout << "Resizing framebuffer (ID: " << _frameBufferID
            << ", dimensions: " << newWidth << " x " << newHeight << ")\n";

  if (_colorRenderBuffer) {
    if (!_colorRenderBuffer->resize(newWidth, newHeight)) {
      std::cerr << "Unable to resize color attachment for the framebuffer (ID: "
                << _frameBufferID << ")\n";
      deleteFrameBuffer();
      return false;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _colorRenderBuffer->getID());
  }

  if (_depthRenderBuffer) {
    if (!_depthRenderBuffer->resize(newWidth, newHeight)) {
      std::cerr << "Unable to resize depth attachment for the framebuffer (ID: "
                << _frameBufferID << ")\n";
      deleteFrameBuffer();
      return false;
    }

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, _depthRenderBuffer->getID());
  }

  if (_texture) {
    if (!_texture->resize(newWidth, newHeight)) {
      std::cerr << "Unable to resize depth attachment for the framebuffer (ID: "
                << _frameBufferID << ")\n";
      deleteFrameBuffer();
      return false;
    }

    _texture->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           _texture->getID(), 0);
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

void FrameBuffer::copyColorToDefaultFrameBuffer(const App& app) const {
  const auto windowWidth = app.getWindowWidth();
  const auto windowHeight = app.getWindowHeight();

  bindAsRead();
  Default::bindAsDraw();
  glBlitFramebuffer(0, 0, _width, _height, 0, 0, windowWidth, windowHeight,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void FrameBuffer::copyDepthFromDefaultFrameBuffer(const App& app) const {
  // TODO: This function should work in theory, but copying depth results in
  // strange visual artifacts. I really don't know what am I doing wrong, I
  // googled through entire internet but to no avail. If anyone has an idea how
  // to make this work properly, let me know really
  const auto windowWidth = app.getWindowWidth();
  const auto windowHeight = app.getWindowHeight();

  Default::bindAsRead();
  bindAsDraw();

  glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, _width, _height,
                    GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

GLsizei FrameBuffer::getWidth() const {
  return _width;
}

GLsizei FrameBuffer::getHeight() const {
  return _height;
}

GLint FrameBuffer::getDepthBits() {
  if (_depthBits != -1 || _frameBufferID == 0) {
    return _depthBits;
  }

  bindAsRead();
  glGetFramebufferAttachmentParameteriv(
      GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &_depthBits);
  const auto error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr << "Unable to not read number of depth bits for framebuffer (ID: "
              << _frameBufferID << ",  error: " << error
              << "). It probably has no depth attachment.\n";
  }

  return _depthBits;
}

GLint FrameBuffer::getStencilBits() {
  if (_stencilBits != -1 || _frameBufferID == 0) {
    return _stencilBits;
  }

  bindAsRead();
  glGetFramebufferAttachmentParameteriv(
      GL_READ_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
      GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &_stencilBits);
  const auto error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr << "Unable to read number of stencil bits for framebuffer (ID: "
              << _frameBufferID << ", error: " << error
              << "). It probably has no stencil attachment.\n";
  }

  return _stencilBits;
}

std::vector<GLubyte> FrameBuffer::readColorValue(int x, int y) {
  std::vector<GLubyte> result(4, 0);
  glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, result.data());
  return result;
}

void FrameBuffer::deleteFrameBuffer() {
  _colorRenderBuffer.reset();
  _depthRenderBuffer.reset();
  deleteOnlyFrameBuffer();
}

Texture* FrameBuffer::getTexture() const {
  return _texture.get();
}

void FrameBuffer::Default::bindAsBothReadAndDraw() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Default::bindAsRead() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBuffer::Default::bindAsDraw() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

GLint FrameBuffer::Default::getDepthBits() {
  GLint depthBits{-1};
  bindAsRead();
  glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, GL_DEPTH,
                                        GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
                                        &depthBits);
  return depthBits;
}

GLint FrameBuffer::Default::getStencilBits() {
  GLint stencilBits{-1};
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

FrameBuffer::Builder& FrameBuffer::Builder::createAndBind(GLsizei width,
                                                          GLsizei height) {
  _frameBuffer = std::make_unique<FrameBuffer>();
  _frameBuffer->createAndBind(width, height);
  return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::withColorAttachment(
    GLenum internalFormat) {
  if (_frameBuffer) {
    _frameBuffer->withColorAttachment(internalFormat);
  }

  return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::withTextureColorAttachment(
    GLenum internalFormat) {
  if (_frameBuffer) {
    _frameBuffer->withTextureColorAttachment(internalFormat);
  }

  return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::withDepthAttachment(
    GLenum internalFormat) {
  if (_frameBuffer) {
    _frameBuffer->withDepthAttachment(internalFormat);
  }

  return *this;
}

std::unique_ptr<FrameBuffer> FrameBuffer::Builder::finishAndGetUnique() {
  if (_frameBuffer == nullptr) {
    return nullptr;
  }

  return _frameBuffer->finishInitialization() ? std::move(_frameBuffer)
                                              : nullptr;
}

std::shared_ptr<FrameBuffer> FrameBuffer::Builder::finishAndGetShared() {
  if (_frameBuffer == nullptr) {
    return nullptr;
  }

  return _frameBuffer->finishInitialization() ? std::move(_frameBuffer)
                                              : nullptr;
}

bool FrameBuffer::createAndBind(GLsizei width, GLsizei height) {
  glGenFramebuffers(1, &_frameBufferID);
  if (_frameBufferID == 0) {
    std::cerr << "Unable to create framebuffer\n";
    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferID);
  std::cout << "Created framebuffer (ID: " << _frameBufferID
            << ", dimensions: " << width << " x " << height << ")\n";
  _width = width;
  _height = height;

  return true;
}

bool FrameBuffer::withColorAttachment(GLenum internalFormat) {
  if (_frameBufferID == 0) {
    return false;
  }

  // Create color render buffer and attach it to FBO
  auto colorRenderBuffer = std::make_unique<RenderBuffer>();
  if (!colorRenderBuffer->create(internalFormat, _width, _height)) {
    std::cerr << "Unable to create color attachment for the framebuffer (ID: "
              << _frameBufferID << ")\n";
    deleteFrameBuffer();
    return false;
  }

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, colorRenderBuffer->getID());
  _colorRenderBuffer = std::move(colorRenderBuffer);
  return true;
}

bool FrameBuffer::withTextureColorAttachment(GLenum internalFormat) {
  if (_frameBufferID == 0) {
    return false;
  }

  // Create an empty texture with no data and same size as framebuffer has
  _texture = std::make_unique<Texture>();
  _texture->createFromData(nullptr, _width, _height, internalFormat, false);
  _texture->bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         _texture->getID(), 0);
  return true;
}

bool FrameBuffer::withDepthAttachment(GLenum internalFormat) {
  // Create depth buffer and attach it to FBO
  auto depthRenderBuffer = std::make_unique<RenderBuffer>();
  if (!depthRenderBuffer->create(internalFormat, _width, _height)) {
    std::cerr << "Unable to create depth attachment for the framebuffer (ID: "
              << _frameBufferID << ")\n";
    deleteFrameBuffer();
    return false;
  }

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depthRenderBuffer->getID());
  _depthRenderBuffer = std::move(depthRenderBuffer);
  return true;
}

bool FrameBuffer::finishInitialization() const {
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

void FrameBuffer::deleteOnlyFrameBuffer() {
  if (_frameBufferID == 0) {
    return;
  }

  std::cout << "Deleting framebuffer (ID: " << _frameBufferID << ")\n";
  glDeleteFramebuffers(1, &_frameBufferID);
  _frameBufferID = 0;
  _width = 0;
  _height = 0;
  _depthBits = -1;
  _stencilBits = -1;
}
