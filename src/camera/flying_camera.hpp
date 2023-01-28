#ifndef FLYING_CAMERA_HPP
#define FLYING_CAMERA_HPP

#include <functional>

#include <glm/glm.hpp>

#include "camera.hpp"

/**
 * Implements a camera,that can fly around the world freely.
 * It cannot rotate more than 90 degrees up/down.
 */
class FlyingCamera : public Camera {
 public:
  FlyingCamera(const glm::vec3& position,
               const glm::vec3& viewPoint,
               const glm::vec3& upVector,
               float moveSpeed = 10.0f,
               float mouseSensitivity = 0.15f);

  /**
   * Sets movement speed of camera (how fast camera moves forward / backward and
   * strafes left / right).
   * @param moveSpeed  Movement speed (distance travelled per second)
   */
  void setMoveSpeed(float moveSpeed);

  /**
   * Sets mouse sensitivity, or the speed, with which you rotate view with
   * mouse.
   * @param mouseSensitivity  Sensitivity in degrees per pixel - how many
   * degrees does camera turn for each pixel moved
   */
  void setMouseSensitivity(float mouseSensitivity);

  /**
   * Gets the camera's current view matrix, calculated from position and
   * viewpoint of camera.
   */
  glm::mat4 getViewMatrix() const override;

  /**
   * Gets the camera's current position.
   */
  glm::vec3 getPosition() const override;

  /**
   * Gets the point camera looks to.
   */
  glm::vec3 getViewPoint() const;

  /**
   * Gets the up vector of the camera.
   */
  glm::vec3 getUpVector() const;

  /**
   * Gets normalized view vector of the camera.
   */
  glm::vec3 getNormalizedViewVector() const;

  /**
   * Updates camera: reacts to key presses and, updates camera's internal state.
   * @param windowSize Current size of the window
   * @param cursorPos Current position of the cursor in the window
   * @param setCursorPosFunc     Function that sets current cursor position
   * @param keyInputFunc Function that detects key presses
   * @param speedCorrectionFunc Function that corrects values based on time
   * passed
   */
  void update(const glm::ivec2& windowSize,
              const glm::ivec2& cursorPos,
              const std::function<void(const glm::i32vec2&)>& setCursorPosFunc,
              const std::function<bool(int)>& keyInputFunc,
              const std::function<float(float)>& speedCorrectionFunc);

 private:
  /**
   * Moves camera by specified distance (positive value = move forward, negative
   * value = move backward).
   * @param distance  Distance to move by
   */
  void moveBy(float distance);

  /**
   * Strafes camera by specified distance (positive value = strafe right,
   * negative value = strafe left).
   * @param distance Distance to strafe by
   */
  void strafeBy(float distance);

  /**
   * Rotates camera view by specified angle to the left or right side.
   * @param angleInDegrees Angle to rotate by (in degrees)
   */
  void rotateLeftRight(float angleInDegrees);

  /**
   * Rotates camera view by specified angle up or down.
   * @param angleInDegrees Angle to rotate by (in degrees)
   */
  void rotateUpDown(float angleInDegrees);

  glm::vec3 _position;   // Camera's position (eye position)
  glm::vec3 _viewPoint;  // Viewpoint (where the camera looks to)
  glm::vec3 _upVector;   // Up vector of the camera

  float _mouseSensitivity;  // Mouse sensitivity in degrees per pixel
  float _moveSpeed;         // Speed of camera (in all directions)
};

#endif