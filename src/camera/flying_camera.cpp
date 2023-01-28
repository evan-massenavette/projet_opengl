#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../keybinds.hpp"

#include "flying_camera.hpp"

FlyingCamera::FlyingCamera(const glm::vec3& position,
                           const glm::vec3& viewPoint,
                           const glm::vec3& upVector,
                           float moveSpeed,
                           float mouseSensitivity)
    : _position(position),
      _viewPoint(viewPoint),
      _upVector(upVector),
      _moveSpeed(moveSpeed),
      _mouseSensitivity(mouseSensitivity) {}

void FlyingCamera::setMoveSpeed(float moveSpeed) {
  _moveSpeed = moveSpeed;
}

void FlyingCamera::setMouseSensitivity(float mouseSensitivity) {
  _mouseSensitivity = mouseSensitivity;
}

glm::mat4 FlyingCamera::getViewMatrix() const {
  return glm::lookAt(_position, _viewPoint, _upVector);
}

glm::vec3 FlyingCamera::getPosition() const {
  return _position;
}

glm::vec3 FlyingCamera::getViewPoint() const {
  return _viewPoint;
}

glm::vec3 FlyingCamera::getUpVector() const {
  return _upVector;
}

void FlyingCamera::update(
    const glm::ivec2& windowSize,
    const glm::ivec2& cursorPos,
    const std::function<void(const glm::i32vec2&)>& setCursorPosFunc,
    const std::function<bool(int)>& keyInputFunc,
    const std::function<float(float)>& speedCorrectionFunc) {
  // Movement
  if (keyInputFunc(Keybinds::forward)) {
    moveBy(speedCorrectionFunc(_moveSpeed));
  }
  if (keyInputFunc(Keybinds::backward)) {
    moveBy(-speedCorrectionFunc(_moveSpeed));
  }
  if (keyInputFunc(Keybinds::left)) {
    strafeBy(-speedCorrectionFunc(_moveSpeed));
  }
  if (keyInputFunc(Keybinds::right)) {
    strafeBy(speedCorrectionFunc(_moveSpeed));
  }

  auto windowCenterPos = windowSize / 2;

  // Rotations
  const auto delta = windowCenterPos - cursorPos;
  if (delta.x != 0) {
    rotateLeftRight(static_cast<float>(delta.x) * _mouseSensitivity);
  }
  if (delta.y != 0) {
    rotateUpDown(static_cast<float>(delta.y) * _mouseSensitivity);
  }

  // Set cursor position to center of window
  setCursorPosFunc(windowCenterPos);
}

void FlyingCamera::moveBy(float distance) {
  glm::vec3 vOffset = getNormalizedViewVector();
  vOffset *= distance;
  _position += vOffset;
  _viewPoint += vOffset;
}

void FlyingCamera::strafeBy(float distance) {
  glm::vec3 strafeVector =
      glm::normalize(glm::cross(getNormalizedViewVector(), _upVector));
  strafeVector = glm::normalize(strafeVector);
  strafeVector *= distance;

  _position += strafeVector;
  _viewPoint += strafeVector;
}

void FlyingCamera::rotateLeftRight(float angleInDegrees) {
  glm::mat4 rotationMatrix =
      glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees),
                  glm::vec3(0.0f, 1.0f, 0.0f));
  glm::vec4 rotatedViewVector =
      rotationMatrix * glm::vec4(getNormalizedViewVector(), 0.0f);
  _viewPoint = _position + glm::vec3(rotatedViewVector);
}

void FlyingCamera::rotateUpDown(float angleInDegrees) {
  const glm::vec3 viewVector = getNormalizedViewVector();
  const glm::vec3 viewVectorNoY =
      glm::normalize(glm::vec3(viewVector.x, 0.0f, viewVector.z));

  float currentAngleDegrees =
      glm::degrees(acos(glm::dot(viewVectorNoY, viewVector)));
  if (viewVector.y < 0.0f) {
    currentAngleDegrees = -currentAngleDegrees;
  }

  float newAngleDegrees = currentAngleDegrees + angleInDegrees;
  if (newAngleDegrees > -85.0f && newAngleDegrees < 85.0f) {
    glm::vec3 rotationAxis = glm::cross(getNormalizedViewVector(), _upVector);
    rotationAxis = glm::normalize(rotationAxis);

    glm::mat4 rotationMatrix = glm::rotate(
        glm::mat4(1.0f), glm::radians(angleInDegrees), rotationAxis);
    glm::vec4 rotatedViewVector =
        rotationMatrix * glm::vec4(getNormalizedViewVector(), 0.0f);

    _viewPoint = _position + glm::vec3(rotatedViewVector);
  }
}

glm::vec3 FlyingCamera::getNormalizedViewVector() const {
  return glm::normalize(_viewPoint - _position);
}
