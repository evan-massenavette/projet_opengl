#include <cstring>
#include <iostream>

#include "vertex_buffer_object.hpp"

void VertexBufferObject::createVBO(size_t reserveSizeBytes) {
  if (isBufferCreated()) {
    std::cerr << "Unable to create vertex buffer object because it's already "
                 "created.\n";
    return;
  }

  glGenBuffers(1, &_bufferID);
  _rawData.reserve(reserveSizeBytes > 0 ? reserveSizeBytes : 1024);
  std::cout << "Created vertex buffer object (ID:  " << _bufferID
            << ", initial reserved size: " << _rawData.capacity()
            << " bytes)\n";
}

void VertexBufferObject::bindVBO(GLenum bufferType) {
  if (!isBufferCreated()) {
    std::cerr
        << "Unable to bind vertex buffer object because it isn't created\n";
    return;
  }

  _bufferType = bufferType;
  glBindBuffer(_bufferType, _bufferID);
}

void VertexBufferObject::unbindVBO() {
  if (!isBufferCreated()) {
    std::cerr
        << "Unable to unbind vertex buffer object because it isn't created\n";
    return;
  }
  glBindBuffer(_bufferType, 0);
}

void VertexBufferObject::addRawData(const void* ptrData,
                                    size_t dataSizeBytes,
                                    size_t repeat) {
  const auto bytesToAdd = dataSizeBytes * repeat;
  const auto requiredCapacity = _bytesAdded + bytesToAdd;

  // If the current capacity isn't sufficient, let's resize the internal vector
  // of raw data
  if (requiredCapacity > _rawData.capacity()) {
    // Determine new raw data buffer capacity - enlarge by a factor of two until
    // it becomes big enough
    auto newCapacity = _rawData.capacity() * 2;
    while (newCapacity < requiredCapacity) {
      newCapacity *= 2;
    }

    // Reserve new vector with higher capacity (which will replace member
    // rawData_) and copy existing data over
    std::vector<unsigned char> newRawData;
    newRawData.reserve(newCapacity);
    memcpy(newRawData.data(), _rawData.data(), _bytesAdded);
    _rawData = std::move(newRawData);
  }

  // Add the data now that we are sure that capacity is sufficient
  for (size_t i = 0; i < repeat; i++) {
    memcpy(_rawData.data() + _bytesAdded, ptrData, dataSizeBytes);
    _bytesAdded += dataSizeBytes;
  }
}

void* VertexBufferObject::getRawDataPointer() {
  return _rawData.data();
}

void VertexBufferObject::uploadDataToGPU(GLenum usageHint) {
  if (!isBufferCreated()) {
    std::cerr << "Unable to upload vertex buffer object data to GPU because it "
                 "isn't created.\n";
    return;
  }

  glBufferData(_bufferType, _bytesAdded, _rawData.data(), usageHint);
  _uploadedDataSize = _bytesAdded;
  _bytesAdded = 0;
}

void* VertexBufferObject::mapBufferToMemory(GLenum usageHint) const {
  if (!isDataUploaded()) {
    return nullptr;
  }

  return glMapBuffer(_bufferType, usageHint);
}

void* VertexBufferObject::mapSubBufferToMemory(GLenum usageHint,
                                               size_t offset,
                                               size_t length) const {
  if (!isDataUploaded()) {
    return nullptr;
  }

  return glMapBufferRange(_bufferType, offset, length, usageHint);
}

void VertexBufferObject::unmapBuffer() const {
  glUnmapBuffer(_bufferType);
}

GLuint VertexBufferObject::getBufferID() const {
  return _bufferID;
}

size_t VertexBufferObject::getBufferSize() {
  return isDataUploaded() ? _uploadedDataSize : _bytesAdded;
}

void VertexBufferObject::deleteVBO() {
  if (!isBufferCreated()) {
    return;
  }

  std::cout << "Deleting vertex buffer object (ID: " << _bufferID << ")\n";
  glDeleteBuffers(1, &_bufferID);
  _bufferID = 0;
  _bytesAdded = 0;
  _uploadedDataSize = 0;
}

bool VertexBufferObject::isBufferCreated() const {
  return _bufferID != 0;
}

bool VertexBufferObject::isDataUploaded() const {
  return _uploadedDataSize > 0;
}
