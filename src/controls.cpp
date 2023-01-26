#include "keybinds.hpp"

#include "controls.hpp"

void Controls::processInputs(GLFWwindow* window) {
  // Exit the app
  if (glfwGetKey(window, Keybinds::exit) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
