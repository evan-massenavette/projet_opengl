#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

class Controls {
private:
  // Initial position : on +Z
  glm::vec3 _position = glm::vec3(0, 0, 5);
  // Initial horizontal angle : toward -Z
  float _horizontalAngle = 3.14f;
  // Initial vertical angle : none
  float _verticalAngle = 0.0f;
  // Initial Field of View
  float _initialFov = 45.0f;

  float _movementSpeed = 3.0f; // 3 units / second
  float _lookSensitivity = 0.005f;

  glm::mat4 _viewMatrix;
  glm::mat4 _projectionMatrix;
  void _computeMatrices(GLFWwindow *window);

public:
  void processInputs(GLFWwindow *window);
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();
};

#endif