#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "following_camera.hpp"

FollowingCamera::FollowingCamera(
    const std::unique_ptr<SceneObject>& sceneObject,
    const glm::vec3& positionOffset,
    const glm::vec3& rotationOffset,
    const glm::vec3& upVector,
    float mouseSensitivity)
    : _sceneObject(sceneObject),
      _positionOffset(positionOffset),
      _rotationOffset(rotationOffset),
      _viewPoint(_sceneObject->getPosition() + glm::vec3(1, 0, 0)),
      _upVector(glm::normalize(upVector)),
      _mouseSensitivity(mouseSensitivity),
      _position(_sceneObject->getPosition()) {}

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
  // Object movement
  const auto objectMovement = getObjectMovement();
  const auto normalizedViewVector = getNormalizedViewVector();

  // Coordinate system for the object
  auto xAxis = normalizedViewVector;
  auto yAxis = _upVector;
  auto zAxis = glm::normalize(glm::cross(xAxis, yAxis));

  auto positionOffset = _positionOffset.x * xAxis + _positionOffset.y * yAxis +
                        _positionOffset.z * zAxis;

  // Set camera positon
  _position = _sceneObject->getPosition() + positionOffset;

  // Set cursor position to center of window
  auto windowCenterPos = windowSize / 2;
  setCursorPosFunc(windowCenterPos);

  // Update attributes
  if (objectMovement != glm::vec3(0)) {
    _viewPoint = _position + normalizedViewVector;
  }
  _viewPoint = _position + normalizedViewVector;
  _lastObjectPosition = _sceneObject->getPosition();
  _lastObjectRotation = _sceneObject->getRotation();
}

glm::vec3 FollowingCamera::getNormalizedViewVector() const {
  return glm::normalize(getObjectMovement());
}

glm::vec3 FollowingCamera::getObjectMovement() const {
  return _sceneObject->getPosition() - _lastObjectPosition;
}
