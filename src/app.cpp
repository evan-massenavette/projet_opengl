#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera/flying_camera.hpp"
#include "camera/following_camera.hpp"
#include "controls.hpp"
#include "renderer.hpp"
#include "scene/scene.hpp"
#include "utils/string_utils.hpp"

#include "app.hpp"

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
    std::cerr << "Unable to initialize GLFW\n";
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
  int defaultWidth = std::max(_minWindowWidth, videoMode->width / 2);
  int defaultHeight = std::max(_minWindowHeight, videoMode->height / 2);

  // Create the window
  _window =
      glfwCreateWindow(defaultWidth, defaultHeight, windowTitle.c_str(),
                       showFullscreen ? primaryMonitor : nullptr, nullptr);
  if (_window == nullptr) {
    std::cerr << "Unable to create GLFW window\n";
    return false;
  }
  glfwMakeContextCurrent(_window);

  // Set the minimum window size
  glfwSetWindowSizeLimits(_window, _minWindowWidth, _minWindowHeight,
                          GLFW_DONT_CARE, GLFW_DONT_CARE);

  // Set app pointer associated with this window
  glfwSetWindowUserPointer(_window, this);

  // Set window's static callbacks
  glfwSetWindowSizeCallback(_window, _onWindowResizeStatic);

  // Manually update the window's size
  glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
  // Hide the mouse and enable unlimited mouvement
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Load GLAD
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cerr << "Unable to initialize GLAD\n";
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

void App::_updateWindowTitle(const std::string& baseTitle,
                             const glm::vec3& cameraPos) {
  // Camera position
  const auto cameraPosStr = string_utils::vecToString(cameraPos);

  // Set the window's title
  const auto newWindowTitle =
      string_utils::formatString("{} | FPS: {} | Position: {} | Speed: {}",
                                 baseTitle, _FPS, cameraPosStr, _movementSpeed);
  glfwSetWindowTitle(_window, newWindowTitle.c_str());
}

void App::run() {
  // Open window
  const std::string baseWindowTitle = "Projet OpenGL Evan & Vincent";
  createWindow(baseWindowTitle.c_str(), 3, 3, false);

  // Init
  setVerticalSync(true);
  _recalculateProjectionMatrix();

  // Update time at the beginning, so that calculations are correct
  _lastFrameTime = _lastFrameTimeFPS = glfwGetTime();

  // Objects used during main loop
  Scene scene(true);
  FlyingCamera flyingCamera(glm::vec3(8, 20, 10), glm::vec3(0, 20, -35),
                            glm::vec3(0, 1, 0));
  auto& cart = scene.objects.front();
  FollowingCamera followingCamera(cart, glm::vec3(0, 1, 0), glm::vec3(0),
                                  glm::vec3(0, 1, 0));
  Controls controls;
  Renderer renderer(*this, scene);

  while (glfwWindowShouldClose(_window) == 0) {
    // Get the right camera based from the controls
    Camera& camera = controls.getCurrentCamera(flyingCamera, followingCamera);

    // Render
    renderer.update(camera);

    // Draw to screen + poll events
    glfwSwapBuffers(_window);
    glfwPollEvents();

    // Show information in window title
    _updateWindowTitle(baseWindowTitle, camera.getPosition());

    // Delta time, FPS and movement speed
    _updateDeltaTimeAndFPS();
    _updateMovementSpeed(camera);

    // Process inputs
    controls.processInputs(*this);

    // Functions used for updates
    auto keyInputFunc = [this](int keyCode) {
      return this->keyPressed(keyCode);
    };
    auto setCursorPosFunc = [this](const glm::i32vec2& pos) {
      glfwSetCursorPos(this->_window, pos.x, pos.y);
    };
    auto speedCorrectionFunc = [this](float f) { return this->saf(f); };

    // Update scene
    scene.update(speedCorrectionFunc);

    // Update cameras
    flyingCamera.update(getWindowSize(), getCursorPosition(), setCursorPosFunc,
                        keyInputFunc, speedCorrectionFunc);
    followingCamera.update(getWindowSize(), getCursorPosition(),
                           setCursorPosFunc);
  }

  destroyWindow();
}

void App::_updateMovementSpeed(Camera& camera) {
  auto currentCameraPos = camera.getPosition();
  _movementSpeed = glm::distance(currentCameraPos, _lastCameraPos) /
                   static_cast<float>(_timeDelta);
  _lastCameraPos = currentCameraPos;
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
  _hasErrorOccurred = hasErrorOccurred;
}

bool App::hasErrorOccurred() const {
  return _hasErrorOccurred;
}

glm::mat4 App::getProjectionMatrix() const {
  return _projectionMatrix;
}

float App::saf(float value) const {
  return value * static_cast<float>(_timeDelta);
}

double App::getTimeDelta() const {
  return _timeDelta;
}

int App::getFPS() const {
  return _FPS;
}

void App::setVerticalSync(bool enable) {
  glfwSwapInterval(enable ? 1 : 0);
  _isVerticalSyncEnabled = enable;
}

bool App::isVerticalSyncEnabled() const {
  return _isVerticalSyncEnabled;
}

int App::getWindowWidth() const {
  return _windowWidth;
}

int App::getWindowHeight() const {
  return _windowHeight;
}

glm::ivec2 App::getCursorPosition() const {
  double posX, posY;
  glfwGetCursorPos(_window, &posX, &posY);
  return glm::ivec2(static_cast<int>(posX), static_cast<int>(posY));
}

glm::ivec2 App::getWindowSize() const {
  // Use cached window width and height
  return glm::ivec2(_windowWidth, _windowHeight);
}

void App::_recalculateProjectionMatrix() {
  float aspectRatio =
      static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight);

  // Other vars used for projection
  float vFov = 60.0f;  // Vertical fov in degrees
  float zNear = 0.1f;
  float zFar = 1500.0f;

  // Projection matrix calculation
  _projectionMatrix =
      glm::perspective(glm::radians(vFov), aspectRatio, zNear, zFar);
}

void App::_updateDeltaTimeAndFPS() {
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

void App::_onWindowResizeInternal(int width, int height) {
  _windowWidth = width;
  _windowHeight = height;
  glViewport(0, 0, _windowWidth, _windowHeight);
  _recalculateProjectionMatrix();
}

App* App::_getAppPtrFromWindow(GLFWwindow* window) {
  auto app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
  return app;
}

void App::_onWindowResizeStatic(GLFWwindow* window, int width, int height) {
  auto app = _getAppPtrFromWindow(window);
  app->_onWindowResizeInternal(width, height);
}
