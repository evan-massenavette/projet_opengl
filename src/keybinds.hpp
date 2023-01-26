#ifndef KEYBINDS_HPP
#define KEYBINDS_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Keybinds {
 public:
  // App control
  static const int exit = GLFW_KEY_ESCAPE;

  // Movement
  static const int forward = GLFW_KEY_W;
  static const int left = GLFW_KEY_A;
  static const int backward = GLFW_KEY_S;
  static const int right = GLFW_KEY_D;

 private:
  // When a keybind in unbound
  static const int unbound = GLFW_KEY_UNKNOWN;
};

#endif