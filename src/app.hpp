#ifndef APP_HPP
#define APP_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class App {
public:
  void run();

private:
  GLFWwindow *_window;

  void _openWindow();
  void _closeWindow();

  static void _processResize(GLFWwindow *window, int width, int height);
};

#endif