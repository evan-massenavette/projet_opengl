#ifndef RENDER_BUFFER_HPP
#define RENDER_BUFFER_HPP

#include <vector>

#include <glad/glad.h>

/**
 * Wraps OpenGL renderbuffer object (resizing, reading)
 */
class RenderBuffer {
 public:
  ~RenderBuffer();

  /**
   * Creates a new renderbuffer with given internal format, width and height.
   * @param internalFormat  Internal format used
   * @param width           Renderbuffer height (in pixels)
   * @param height          Renderbuffer height (in pixels)   *
   * @return True if the renderbuffer has been created successfully, false
   * otherwise
   */
  bool create(GLenum internalFormat, GLsizei width, GLsizei height);

  /**
   * Resizes the renderbuffer to new width and height, while keeping its
   * original internal format. Does nothing if the renderbuffer hasn't been
   * created before.
   * @param newWidth   New renderbuffer width (in pixels)
   * @param newHeight  New renderbuffer height (in pixels)
   * @return True if the renderbuffer has been resized successfully, false
   * otherwise
   */
  bool resize(GLsizei newWidth, GLsizei newHeight);

  /**
   * Gets OpenGL-assigned renderbuffer ID.
   */
  GLuint getID() const;

  /**
   * Gets internal storage format of the renderbuffer.
   */
  GLenum getInternalFormat() const;

  /**
   * Gets renderbuffer width (in pixels).
   */
  GLsizei getWidth() const;

  /**
   * Gets renderbuffer height (in pixels).
   */
  GLsizei getHeight() const;

  /**
   * Queries, caches and returns how many bits are used to represent depth in
   * this renderbuffer.
   */
  GLint getDepthBits();

  /**
   * Queries, caches and returns how many bits are used to represent stencil in
   * this renderbuffer.
   */
  GLint getStencilBits();

  /**
   * Deletes renderbuffer OpenGL object and resets class to its original state.
   */
  void deleteRenderBuffer();

 private:
  GLuint _renderBufferID = 0;  // OpenGL-assigned ID of the renderbuffer
  GLenum _internalFormat = 0;  // Internal format selected during creation
  GLsizei _width = 0;          // Width of the renderbuffer
  GLsizei _height = 0;         // Height of the renderbuffer

  GLint _depthBits = 0;    // Cached number of bits for depth
  GLint _stencilBits = 0;  // Cached number of bits for stencil
};

#endif
