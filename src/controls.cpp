#include <glm/gtc/matrix_transform.hpp>

#include "controls.hpp"

void Controls::processInputs(GLFWwindow *window) {
  // Exit the app
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  _computeMatrices(window);
}

void Controls::_computeMatrices(GLFWwindow *window) {

  // Get window size
  int windowWidth, windowHeight;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);

  // glfwGetTime is called only once, the first time this function is called
  static double lastTime = glfwGetTime();

  // Compute time difference between current and last frame
  double currentTime = glfwGetTime();
  float deltaTime = float(currentTime - lastTime);

  // Get mouse position
  double cursorX, cursorY;
  glfwGetCursorPos(window, &cursorX, &cursorY);

  // Reset mouse position for next frame
  glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);

  // Compute new orientation
  _horizontalAngle += _lookSensitivity * float(windowWidth / 2 - cursorX);
  _verticalAngle += _lookSensitivity * float(windowHeight / 2 - cursorY);

  // Direction : Spherical coordinates to Cartesian coordinates conversion
  glm::vec3 direction(cos(_verticalAngle) * sin(_horizontalAngle),
                      sin(_verticalAngle),
                      cos(_verticalAngle) * cos(_horizontalAngle));

  // Right vector
  glm::vec3 right = glm::vec3(sin(_horizontalAngle - 3.14f / 2.0f), 0,
                              cos(_horizontalAngle - 3.14f / 2.0f));

  // Up vector
  glm::vec3 up = glm::cross(right, direction);

  // Move forward
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    _position += direction * deltaTime * _movementSpeed;
  }
  // Move backward
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    _position -= direction * deltaTime * _movementSpeed;
  }
  // Strafe right
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    _position += right * deltaTime * _movementSpeed;
  }
  // Strafe left
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    _position -= right * deltaTime * _movementSpeed;
  }

  float FoV =
      _initialFov; // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting
                   // up a callback for this. It's a bit too complicated for
                   // this beginner's tutorial, so it's disabled instead.

  // Projection matrix
  float aspect_ratio = (float)windowWidth / (float)windowHeight;
  float z_near = 0.1f;  // Near clipping plane
  float z_far = 100.0f; // Far clipping plane
  _projectionMatrix =
      glm::perspective(glm::radians(FoV), aspect_ratio, z_near, z_far);

  // Camera matrix
  _viewMatrix = glm::lookAt(_position, _position + direction, up);

  // For the next frame, the "last time" will be "now"
  lastTime = currentTime;
}

glm::mat4 Controls::getViewMatrix() const { return _viewMatrix; }
glm::mat4 Controls::getProjectionMatrix() const { return _projectionMatrix; }
