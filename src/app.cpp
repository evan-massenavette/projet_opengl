#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "app.hpp"
#include "controls.hpp"
#include "flying_camera.hpp"
#include "renderer.hpp"
#include "scene/scene.hpp"
#include "texture.hpp"

App::App() {
  // Set all keys as not pressed
  for (auto& kwp : _keyWasPressed) {
    kwp = false;
  }
}

bool App::createWindow(const std::string& windowTitle,
                       int majorVersion,
                       int minorVersion,
                       bool showFullscreen) {
  // Initialize and configure GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return false;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  const auto primaryMonitor = glfwGetPrimaryMonitor();
  const auto videoMode = glfwGetVideoMode(primaryMonitor);

  // Default window size is half of screen's resolution
  // (but no smaller than minimum size)
  int defaultWidth = std::max(minScreenWidth_, videoMode->width / 2);
  int defaultHeight = std::max(minScreenHeight_, videoMode->height / 2);

  // Create the window
  _window =
      glfwCreateWindow(defaultWidth, defaultHeight, windowTitle.c_str(),
                       showFullscreen ? primaryMonitor : nullptr, nullptr);
  if (_window == nullptr) {
    std::cerr << "Failed to create GLFW window\n";
    return false;
  }
  glfwMakeContextCurrent(_window);

  // Set the minimum window size
  glfwSetWindowSizeLimits(_window, minScreenWidth_, minScreenHeight_,
                          GLFW_DONT_CARE, GLFW_DONT_CARE);

  // Set app pointer associated with this window
  glfwSetWindowUserPointer(_window, this);

  // Set window's static callbacks
  glfwSetWindowSizeCallback(_window, onWindowSizeChangedStatic);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
  // Hide the mouse and enable unlimited mouvement
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Load GLAD
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cerr << "Failed to initialize GLAD\n";
    return false;
  }

  return true;
}

void App::destroyWindow() {
  glfwDestroyWindow(_window);
  glfwTerminate();
}

GLFWwindow* App::getWindow() const {
  return _window;
}

void App::run() {
  // Open window
  createWindow("Projet OpenGL Evan & Vincent", 3, 3, false);

  // Init
  setVerticalSynchronization(true);
  recalculateProjectionMatrix();

  // Update time at the beginning, so that calculations are correct
  _lastFrameTime = _lastFrameTimeFPS = glfwGetTime();

  // Objects used during main loop
  Scene scene(glm::vec4(0.0, 0.0, 0.2, 1.0), true);
  FlyingCamera camera(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0),
                      glm::vec3(0, 1, 0));
  Renderer renderer(scene);
  Controls controls;

  while (glfwWindowShouldClose(_window) == 0) {
    updateDeltaTimeAndFPS();

    // Render
    renderer.update(getProjectionMatrix(), camera.getViewMatrix());

    // Draw to screen + poll events
    glfwSwapBuffers(_window);
    glfwPollEvents();

    // Process inputs
    controls.processInputs(_window);

    // Update camera
    auto keyInputFunc = [this](int keyCode) {
      return this->keyPressed(keyCode);
    };
    auto getCursorPosFunc = [this]() {
      double curPosX, curPosY;
      glfwGetCursorPos(this->_window, &curPosX, &curPosY);
      return glm::u32vec2(curPosX, curPosY);
    };
    auto setCursorPosFunc = [this](const glm::i32vec2& pos) {
      glfwSetCursorPos(this->_window, pos.x, pos.y);
    };
    auto speedCorrectionFunc = [this](float f) { return this->sof(f); };
    camera.update(keyInputFunc, getCursorPosFunc, setCursorPosFunc,
                  speedCorrectionFunc);

    // Update scene
  }

  destroyWindow();
}

bool App::keyPressed(int keyCode) const {
  return glfwGetKey(_window, keyCode) == GLFW_PRESS;
}

bool App::keyPressedOnce(int keyCode) {
  bool result = false;
  if (keyPressed(keyCode)) {
    if (!_keyWasPressed[keyCode]) {
      result = true;
    }

    _keyWasPressed[keyCode] = true;
  } else {
    _keyWasPressed[keyCode] = false;
  }

  return result;
}

void App::closeWindow(bool hasErrorOccurred) {
  glfwSetWindowShouldClose(_window, true);
  hasErrorOccurred_ = hasErrorOccurred;
}

bool App::hasErrorOccurred() const {
  return hasErrorOccurred_;
}

glm::mat4 App::getProjectionMatrix() const {
  return _projectionMatrix;
}

float App::sof(float value) const {
  return value * static_cast<float>(_timeDelta);
}

double App::getTimeDelta() const {
  return _timeDelta;
}

int App::getFPS() const {
  return _FPS;
}

void App::setVerticalSynchronization(bool enable) {
  glfwSwapInterval(enable ? 1 : 0);
  _isVerticalSynchronizationEnabled = enable;
}

bool App::isVerticalSynchronizationEnabled() const {
  return _isVerticalSynchronizationEnabled;
}

int App::getScreenWidth() const {
  return screenWidth_;
}

int App::getScreenHeight() const {
  return screenHeight_;
}

glm::ivec2 App::getCursorPosition() const {
  double posX, posY;
  glfwGetCursorPos(_window, &posX, &posY);
  return glm::ivec2(static_cast<int>(posX),
                    screenHeight_ - static_cast<int>(posY));
}

void App::recalculateProjectionMatrix() {
  // App width, height and aspect ratio
  int width, height;
  glfwGetWindowSize(_window, &width, &height);
  float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

  // Other vars used for projection
  float vFov = 60.0f;  // Vertical fov in degrees
  float zNear = 0.1f;
  float zFar = 1500.0f;

  // Projection matrix calculation
  _projectionMatrix =
      glm::perspective(glm::radians(vFov), aspectRatio, zNear, zFar);
}

void App::updateDeltaTimeAndFPS() {
  const auto currentTime = glfwGetTime();
  _timeDelta = currentTime - _lastFrameTime;
  _lastFrameTime = currentTime;
  _nextFPS++;

  if (currentTime - _lastFrameTimeFPS > 1.0) {
    _lastFrameTimeFPS = currentTime;
    _FPS = _nextFPS;
    _nextFPS = 0;
  }
}

void App::onWindowSizeChangedInternal(int width, int height) {
  recalculateProjectionMatrix();
  glViewport(0, 0, width, height);
  screenWidth_ = width;
  screenHeight_ = height;
}

App* App::getAppPtrFromWindow(GLFWwindow* window) {
  auto app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
  return app;
}

void App::onWindowSizeChangedStatic(GLFWwindow* window, int width, int height) {
  auto app = getAppPtrFromWindow(window);
  app->onWindowSizeChangedInternal(width, height);
}
