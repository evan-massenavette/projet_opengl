#include <iostream>

#include "uniform_buffer_object.hpp"

UniformBufferObject::~UniformBufferObject() {
  deleteUBO();
}

void UniformBufferObject::createUBO(const size_t byteSize, GLenum usageHint) {
  if (_isBufferCreated) {
    std::cerr << "Unable to create uniform buffer object because it's already "
                 "created.\n";
    return;
  }

  // Generate buffer ID, bind it immediately and reserve space for it
  glGenBuffers(1, &_bufferID);
  glBindBuffer(GL_UNIFORM_BUFFER, _bufferID);
  glBufferData(GL_UNIFORM_BUFFER, byteSize, NULL, usageHint);

  // Mark that the buffer has been created and store its size
  _isBufferCreated = true;
  _byteSize = byteSize;

  std::cout << "Created uniform buffer object (ID: " << _bufferID
            << ", size: " << _byteSize << " bytes)\n";
}

void UniformBufferObject::bindUBO() const {
  if (!_isBufferCreated) {
    std::cerr
        << "Unable to bind uniform buffer object because it isn't created.\n";
    return;
  }

  glBindBuffer(GL_UNIFORM_BUFFER, _bufferID);
}

void UniformBufferObject::unbindUBO() const {
  if (!_isBufferCreated) {
    std::cerr
        << "Unable to unbind uniform buffer object because it isn't created.\n";
    return;
  }

  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::setBufferData(const size_t offset,
                                        const void* ptrData,
                                        const size_t dataSize) {
  if (!_isBufferCreated) {
    std::cerr << "Unable to set data of uniform buffer object because it isn't "
                 "created.\n";
    return;
  }

  if (offset >= _byteSize) {
    std::cerr << "Unable to set data of uniform buffer object because the "
                 "offset is beyond the buffer's size (offset: "
              << offset << ",  buffer size: " << _byteSize
              << "). Will be ignored.\n";
    return;
  }

  if (offset + dataSize > _byteSize) {
    std::cerr << "Unable to set data of uniform buffer object because it would "
                 "overflow the buffer (offset: "
              << offset << ", data size: " << dataSize << ")\n";
    return;
  }

  glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, ptrData);
}

void UniformBufferObject::bindBufferBaseToBindingPoint(
    const GLuint bindingPoint) const {
  if (!_isBufferCreated) {
    std::cerr << "Unable to bind buffer base to binding point because it isn't "
                 "created (binding point: "
              << bindingPoint << ")\n";
    return;
  }

  glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _bufferID);
}

GLuint UniformBufferObject::getBufferID() const {
  return _bufferID;
}

void UniformBufferObject::deleteUBO() {
  if (!_isBufferCreated) {
    return;
  }

  std::cout << "Deleting uniform buffer object (ID: " << _bufferID << ")\n";
  glDeleteBuffers(1, &_bufferID);
  _isBufferCreated = false;
}
