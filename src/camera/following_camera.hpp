#ifndef FOLLOWING_CAMERA_HPP
#define FOLLOWING_CAMERA_HPP

#include <functional>

#include <glm/glm.hpp>

#include "../scene/scene_object.hpp"

#include "camera.hpp"

/**
 * Implements a camera that follows an object in the scene and looks in the
 * direction it is moving.
 */
class FollowingCamera : public Camera {
 public:
  FollowingCamera(const SceneObject& sceneObject,
                  const glm::vec3& positionOffset,
                  const glm::vec3& rotationOffset,
                  const glm::vec3& initialViewPoint,
                  const glm::vec3& upVector,
                  float mouseSensitivity = 0.15f);

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
   * @param speedCorrectionFunc Function that corrects values based on time
   * passed
   */
  void update(const glm::ivec2& windowSize,
              const glm::ivec2& cursorPos,
              const std::function<void(const glm::i32vec2&)>& setCursorPosFunc);

 private:
  const SceneObject& _sceneObject;  // Object the camera is attached to

  glm::vec3 _lastObjectPosition;
  glm::vec3 _lastObjectRotation;

  glm::vec3 _positionOffset;  // Camera's position offset compared to the object
  glm::vec3 _rotationOffset;  // Camera's rotation offset compared to the object

  glm::vec3 _position;   // Position of the camera
  glm::vec3 _viewPoint;  // Viewpoint (where the camera looks to)
  glm::vec3 _upVector;   // Up vector of the camera

  float _mouseSensitivity;  // Mouse sensitivity in degrees per pixel
  float _moveSpeed;         // Speed of camera (in all directions)
};

#endif