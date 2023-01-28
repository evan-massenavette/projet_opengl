#ifndef KEYBINDS_HPP
#define KEYBINDS_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Keybinds {
 public:
  // App control
  static const int exitApp = GLFW_KEY_ESCAPE;

  // Movement
  static const int moveForward = GLFW_KEY_W;
  static const int moveLeft = GLFW_KEY_A;
  static const int moveBackward = GLFW_KEY_S;
  static const int moveRight = GLFW_KEY_D;

  // Camera
  static const int changeCameraType = GLFW_KEY_C;

 private:
  // When a keybind in unbound
  static const int unbound = GLFW_KEY_UNKNOWN;
};

#endif