#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

/**
 * Abstract class that represents a camera
 */
class Camera {
 public:
  virtual ~Camera() = default;

  /**
   * Gets the camera's current view matrix.
   */
  virtual glm::mat4 getViewMatrix() const = 0;

  /**
   * Gets the camera's current position.
   */
  virtual glm::vec3 getPosition() const = 0;

 protected:
  Camera() = default;
};

#endif