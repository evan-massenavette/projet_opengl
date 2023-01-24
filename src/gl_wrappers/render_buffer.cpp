#include <iostream>

#include "render_buffer.hpp"

RenderBuffer::~RenderBuffer() {
  deleteRenderBuffer();
}

bool RenderBuffer::create(GLenum internalFormat,
                          GLsizei width,
                          GLsizei height) {
  glGenRenderbuffers(1, &_renderBufferID);
  if (_renderBufferID == 0) {
    std::cerr << "Unable to create renderbuffer (internal format: "
              << internalFormat << ", dimensions " << width << " x " << height
              << ")\n";
    return false;
  }

  std::cout << "Created renderbuffer (ID: " << _renderBufferID << ")\n";

  // Bind newly created renderbuffer and set its storage attributes
  glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferID);
  glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);

  // Cache the attributes as member variables
  _internalFormat = internalFormat;
  _width = width;
  _height = height;

  return true;
}

bool RenderBuffer::resize(GLsizei newWidth, GLsizei newHeight) {
  // If the renderbuffer has not been created, we can't even resize it
  if (_renderBufferID == 0) {
    return false;
  }

  // Just remember the internal format, delete and recreate the buffer with new
  // width and height
  const auto internalFormat = _internalFormat;
  deleteRenderBuffer();
  return create(internalFormat, newWidth, newHeight);
}

void RenderBuffer::deleteRenderBuffer() {
  if (_renderBufferID == 0) {
    return;
  }

  std::cout << "Deleting renderbuffer (ID: " << _renderBufferID << ")\n";
  glDeleteRenderbuffers(1, &_renderBufferID);
  _renderBufferID = 0;
  _width = 0;
  _height = 0;
  _depthBits = -1;
  _stencilBits = -1;
}

GLuint RenderBuffer::getID() const {
  return _renderBufferID;
}

GLenum RenderBuffer::getInternalFormat() const {
  return _internalFormat;
}

GLsizei RenderBuffer::getWidth() const {
  return _width;
}

GLsizei RenderBuffer::getHeight() const {
  return _height;
}

GLint RenderBuffer::getDepthBits() {
  if (_depthBits != -1 || _renderBufferID == 0) {
    return _depthBits;
  }

  // Query how many bits are used for depth and cache the result
  glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferID);
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE,
                               &_depthBits);
  return _depthBits;
}

GLint RenderBuffer::getStencilBits() {
  if (_stencilBits != -1 || _renderBufferID == 0) {
    return _stencilBits;
  }

  // Query how many bits are used for stencil and cache the result
  glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferID);
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_STENCIL_SIZE,
                               &_stencilBits);
  return _stencilBits;
}
