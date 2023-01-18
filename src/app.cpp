#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "app.hpp"
#include "controls.hpp"
#include "renderer.hpp"
#include "scene/scene.hpp"
#include "shaders.hpp"
#include "texture.hpp"

void App::_processResize(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  printf("Window resized to %d x %d\n", width, height);
}

void App::_openWindow() {
  // Initialize and configure GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    getchar();
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window
  _window = glfwCreateWindow(1024, 768, "HGL", NULL, NULL);
  if (!_window) {
    std::cerr << "Failed to create GLFW window\n";
    getchar();
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(_window);
  glfwSetFramebufferSizeCallback(_window, _processResize);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
  // Hide the mouse and enable unlimited mouvement
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    getchar();
    exit(EXIT_FAILURE);
  }
}

void App::_closeWindow() {
  // Close OpenGL window and terminate GLFW
  glfwDestroyWindow(_window);
  glfwTerminate();
}

void App::run() {
  // Open the application window
  _openWindow();

  Controls controls;
  Renderer renderer;
  Scene scene;

  // Main loop
  while (!glfwWindowShouldClose(_window)) {
    // Process inputs
    controls.processInputs(_window);

    // Matrices from the controls
    glm::mat4 projection = controls.getProjectionMatrix();
    glm::mat4 view = controls.getViewMatrix();

    // Update the render
    renderer.update(scene, projection, view);

    // Swap buffers
    glfwSwapBuffers(_window);
    glfwPollEvents();
  }

  _closeWindow();
}