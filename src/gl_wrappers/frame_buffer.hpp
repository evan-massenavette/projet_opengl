#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include <memory>
#include <vector>

#include "../app.hpp"
#include "render_buffer.hpp"
#include "texture.hpp"
#include "texture_cube_map.hpp"

/**
 * Wraps OpenGL framebuffer (copying data to / from framebuffer)
 * There is also a static "Default" class to work with the default framebuffer.
 */
class FrameBuffer {
 public:
  ~FrameBuffer();

  bool create(GLsizei width, GLsizei height);

  bool addTexture(GLenum internalFormat, GLenum attachment);
  bool addTextureCubeMap(GLenum internalFormat, GLenum attachment);

  bool isComplete() const;

  void bindAsReadAndDraw() const;
  void bindAsRead() const;
  void bindAsDraw() const;

  /**
   * Resizes yjr framebuffer with new width and height, while keeping
   * its original attachments.
   * @param newWidth   New framebuffer width (in pixels)
   * @param newHeight  New framebuffer height (in pixels)
   * @return True if framebuffer has been resized successfully, false
   * otherwise
   */
  bool resize(GLsizei newWidth, GLsizei newHeight);

  /**
   * Sets viewport to cover whole framebuffer.
   */
  void setFullViewport() const;

  /**
   * Gets width of the framebuffer in pixels.
   */
  GLsizei getWidth() const;

  /**
   * Gets height of the framebuffer in pixels.
   */
  GLsizei getHeight() const;

  /**
   * Reads the color value from the currently bound read framebuffer
   * @return GLubyte vector containing color RGBA values
   */
  static std::vector<GLubyte> readColorValue(int x, int y);

  /**
   * Deletes all OpenGL objects created within this instance of framebuffer
   * and resets class to its initial state.
   */
  void deleteFrameBuffer();

  Texture* getTexture() const;
  TextureCubeMap* getTextureCubeMap() const;

  /**
   * Static subclass for working with default window framebuffer.
   */
  class Default {
   public:
    static void bindAsReadAndDraw();
    static void bindAsRead();
    static void bindAsDraw();

    /**
     * Queries and returns how many bits are used to represent depth in the
     * default framebuffer.
     */
    static GLint getDepthBits();

    /**
     * Queries and returns how many bits are used to represent stencil in the
     * default framebuffer.
     */
    static GLint getStencilBits();

    /**
     * Sets the viewport of default framebuffer to take the whole screen.
     */
    static void setFullViewport(const App& app);
  };

 private:
  GLuint _frameBufferID = 0;  // OpenGL-assignedd ID of the framebuffer
  std::unique_ptr<Texture>
      _texture;  // The texture if framebuffer is used to render one
  std::unique_ptr<TextureCubeMap>
      _textureCubeMap;  // The texture cube map,
                        // if the framebuffer is used to render one

  GLsizei _width = 0;   // Width of the framebuffer in pixels
  GLsizei _height = 0;  // Height of the framebuffer in pixels

  /**
   * Internal method that deletes only framebuffer object, but keeps
   * renderbuffers and texture.
   */
  void deleteOnlyFrameBuffer();
};

#endif
