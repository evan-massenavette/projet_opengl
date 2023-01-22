#include "controls.hpp"

void Controls::processInputs(GLFWwindow* window) {
  // Exit the app
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
