#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "app.hpp"
#include "controls.hpp"
#include "renderer.hpp"
#include "shader.hpp"
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

  // Main loop
  while (!glfwWindowShouldClose(_window)) {
    // Process inputs
    controls.processInputs(_window);

    // Compute MVP matrix from the controls
    glm::mat4 projectionMatrix = controls.getProjectionMatrix();
    glm::mat4 viewMatrix = controls.getViewMatrix();
    auto scale = 1.0;
    glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(scale));
    glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

    // Update the render
    renderer.update(mvp);

    // Swap buffers
    glfwSwapBuffers(_window);
    glfwPollEvents();
  }

  _closeWindow();
}