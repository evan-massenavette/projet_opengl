#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <CImg.h>
using namespace cimg_library;

#include "texture.hpp"

GLuint Texture::load(const char *filepath) {

  printf("Loading texture: %s\n", filepath);

  // Read image file
  CImg<unsigned char> textureImage(filepath);

  // Create texture and bind it
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage.width(),
               textureImage.height(), 0, GL_BGR, GL_UNSIGNED_BYTE,
               textureImage.data());

  // Texture properties
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Poor filtering
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Nice trilinear filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  // Generate mipmaps automatically
  glGenerateMipmap(GL_TEXTURE_2D);

  // Return the ID of the texture we just created
  return texture;
}