#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include <memory>
#include <vector>

#include "../app.hpp"
#include "render_buffer.hpp"
#include "texture.hpp"

/**
 * Wraps OpenGL framebuffer (copying data to / from framebuffer)
 * There is also a static "Default" class to work with the default framebuffer.
 */
class FrameBuffer {
 public:
  ~FrameBuffer();

  bool createFrameBufferWithColorAndDepth(GLsizei width, GLsizei height);

  /**
   * Binds the framebuffer as either read, draw, or both.
   */
  void bindAsBothReadAndDraw() const;
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
   * Copies color values to the default framebuffer.
   */
  void copyColorToDefaultFrameBuffer(const App& app) const;

  /**
   * Copies depth from the default framebuffer.
   */
  void copyDepthFromDefaultFrameBuffer(const App& app) const;

  /**
   * Gets width of the framebuffer in pixels.
   */
  GLsizei getWidth() const;

  /**
   * Gets height of the framebuffer in pixels.
   */
  GLsizei getHeight() const;

  /**
   * Queries, caches and returns how many bits are used to represent depth in
   * this framebuffer. This value will be successfully queried only if
   * framebuffer contains depth component.
   */
  GLint getDepthBits();

  /**
   * Queries, caches and returns how many bits are used to represent stencil in
   * this framebuffer. This value will be successfully queried only if
   * framebuffer contains stencil component.
   */
  GLint getStencilBits();

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

  /**
   * Static subclass for working with default window framebuffer.
   */
  class Default {
   public:
    /**
     * Binds default frame buffer as either read, draw, or both.
     */
    static void bindAsBothReadAndDraw();
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

  /**
   * Subclass that builds a framebuffer object. Because of variety of options,
   * it's a lot more practical to create and configure framebuffer like this.
   */
  class Builder {
   public:
    /**
     * Creates and binds a new framebuffer.
     * @param width New framebuffer width (in pixels)
     * @param height New framebuffer height (in pixels)
     */
    Builder& createAndBind(GLsizei width, GLsizei height);

    /**
     * Adds color attachment with specified format.
     * @param internalFormat Internal format of the color data (like GL_RGB8)
     */
    Builder& withColorAttachment(GLenum internalFormat);

    /**
     * Adds texture color attachment to the framebuffer with specified format.
     * @param internalFormat Internal format of the texture data (like GL_RGB)
     */
    Builder& withTextureColorAttachment(GLenum internalFormat);

    /**
     * Adds depth attachment to the framebuffer.
     * @param internalFormat Internal format of the depth data
     */
    Builder& withDepthAttachment(GLenum internalFormat);

    /**
     * Finishes building and returns unique pointer to the built framebuffer.
     */
    std::unique_ptr<FrameBuffer> finishAndGetUnique();

    /**
     * Finishes building and returns shared pointer to built framebuffer.
     */
    std::shared_ptr<FrameBuffer> finishAndGetShared();

   private:
    std::unique_ptr<FrameBuffer>
        _frameBuffer;  // Holds framebuffer that is being built
  };

 private:
  friend class Builder;  // Builder has access to the private methods below

  bool createAndBind(GLsizei width, GLsizei height);
  bool withColorAttachment(GLenum internalFormat);
  bool withDepthAttachment(GLenum internalFormat);
  bool withTextureColorAttachment(GLenum internalFormat);
  bool finishInitialization() const;

  GLuint _frameBufferID = 0;  // OpenGL-assignedd ID of the framebuffer
  std::unique_ptr<RenderBuffer> _colorRenderBuffer;  // Held color renderbuffer
  std::unique_ptr<RenderBuffer> _depthRenderBuffer;  // Held renderbuffer
  std::unique_ptr<Texture>
      _texture;  // The texture if framebuffer is used to render one

  GLsizei _width = 0;      // Width of the framebuffer in pixels
  GLsizei _height = 0;     // Height of the framebuffer in pixels
  GLint _depthBits = 1;    // Cached number of bits for depth
  GLint _stencilBits = 1;  // Cached number of bits for stencil

  /**
   * Internal method that deletes only framebuffer object, but keeps
   * renderbuffers and texture.
   */
  void deleteOnlyFrameBuffer();
};

#endif
