#ifndef TEXTURE_CUBE_MAP_HPP
#define TEXTURE_CUBE_MAP_HPP

#include <string>

#include <glad/glad.h>

/**
 *  Wraps OpenGL texture into convenient class.
 */
class TextureCubeMap {
 public:
  ~TextureCubeMap();

  /**
   * Creates a texture cube map with the given width, height and format
   * @param width
   * @param height
   * @param format
   * @return True if the texture cube map was successfully create, false
   * otherwise.
   */
  bool create(GLsizei width, GLsizei height, GLenum format);

  /**
   * Binds texture cube map to specified texture unit.
   * @param textureUnit  Texture unit index (default is 0)
   */
  void bind(GLenum textureUnit = 0) const;

  /**
   * Unbinds texture cube map from specified texture unit.
   * @param textureUnit  Texture unit index (default is 0)
   */
  void unbind(GLenum textureUnit = 0) const;

  /**
   * Deletes loaded texture from OpenGL. Does nothing if the texture has not
   * been loaded correctly.
   */
  void deleteTexture();

  /**
   * Gets OpenGL-assigned texture ID
   */
  GLuint getID() const;

  /**
   * Gets width of the texture (in pixels).
   */
  GLsizei getWidth() const;

  /**
   * Gets height of the texture (in pixels).
   */
  GLsizei getHeight() const;

  bool isLoaded() const;

  bool resize(GLsizei newWidth, GLsizei newHeight);

 private:
  GLuint _textureID = 0;  // OpenGL-assigned texture ID
  GLsizei _width = 0;     // Width of texture in pixels
  GLsizei _height = 0;    // Height of texture in pixels
  GLenum _format = 0;     // Format this texture is represented with

  /**
   * Checks, if the texture has been loaded correctly and if not, logs it into
   * console.
   *
   * @return True, if texture has been loaded correctly or false otherwise.
   */
  bool _isLoadedLogged() const;
};

#endif