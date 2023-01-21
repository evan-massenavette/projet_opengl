#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

class Controls {
 public:
  void processInputs(GLFWwindow* window);
};

#endif