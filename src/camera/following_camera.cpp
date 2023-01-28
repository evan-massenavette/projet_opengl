#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "following_camera.hpp"

FollowingCamera::FollowingCamera(const SceneObject& sceneObject,
                                 const glm::vec3& positionOffset,
                                 const glm::vec3& rotationOffset,
                                 const glm::vec3& initialViewPoint,
                                 const glm::vec3& upVector,
                                 float mouseSensitivity)
    : _sceneObject(sceneObject),
      _positionOffset(positionOffset),
      _rotationOffset(rotationOffset),
      _viewPoint(initialViewPoint),
      _upVector(upVector),
      _mouseSensitivity(mouseSensitivity) {}

void FollowingCamera::setMouseSensitivity(float mouseSensitivity) {
  _mouseSensitivity = mouseSensitivity;
}

glm::mat4 FollowingCamera::getViewMatrix() const {
  return glm::lookAt(_position, _viewPoint, _upVector);
}

glm::vec3 FollowingCamera::getPosition() const {
  return _position;
}

glm::vec3 FollowingCamera::getViewPoint() const {
  return _viewPoint;
}

glm::vec3 FollowingCamera::getUpVector() const {
  return _upVector;
}

void FollowingCamera::update(
    const glm::ivec2& windowSize,
    const glm::ivec2& cursorPos,
    const std::function<void(const glm::i32vec2&)>& setCursorPosFunc) {
  // Coordinate system for the object
  auto xAxis = glm::normalize(glm::cross(getNormalizedViewVector(), _upVector));
  auto yAxis = glm::normalize(_upVector);
  auto zAxis = glm::normalize(glm::cross(xAxis, yAxis));

  auto positionOffset = _positionOffset.x * xAxis + _positionOffset.y * yAxis +
                        _positionOffset.z * zAxis;

  // auto rotationOffset = getNormalizedViewVector();

  // Set camera positon
  _position = _sceneObject.getPosition() + positionOffset;

  // Object movement and rotation
  const glm::vec3 objectMovement =
      _sceneObject.getPosition() - _lastObjectPosition;

  // Update camera's view point if the object has moved
  if (objectMovement != glm::vec3(0)) {
    _viewPoint = _position + glm::normalize(objectMovement);
  }

  // Set cursor position to center of window
  auto windowCenterPos = windowSize / 2;
  setCursorPosFunc(windowCenterPos);

  // Update attributes
  _lastObjectPosition = _sceneObject.getPosition();
  _lastObjectRotation = _sceneObject.getRotation();
}

glm::vec3 FollowingCamera::getNormalizedViewVector() const {
  return glm::normalize(_viewPoint - _position);
}
