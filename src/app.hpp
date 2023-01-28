#ifndef APP_HPP
#define APP_HPP

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

class App {
 public:
  /**
   * Constructor of the class, initializes internal structures.
   */
  App();

  /**
   * Creates a window with OpenGL context with given title and context version.
   *  @param windowTitle Title window to create
   *  @param majorVersion OpenGL context major version
   *  @param minorVersion OpenGL context minor version
   *  @param showFullscreen Whether the window should be shown in fullscreen
   *  @return True if window has been created successfully, false otherwise
   */
  bool createWindow(const std::string& windowTitle,
                    int majorVersion,
                    int minorVersion,
                    bool showFullscreen);

  void destroyWindow();

  /**
   * Gets the pointer to the app's window.
   * @return Pointer to GLFW window or nullptr, if the window does not exist.
   */
  GLFWwindow* getWindow() const;

  /**
   * Runs the whole application (main loop).
   */
  void run();

  /**
   * Checks if the specified key is currently pressed.
   * @param keyCode GLFW code of the key to check
   * @return True if key is pressed, false otherwise
   */
  bool keyPressed(int keyCode) const;

  /**
   * Checks if the specified key was pressed. This function won't return true
   * again unless the key has been released and pressed again.
   * @param keyCode GLFW code of the key to check
   * @return True if key was pressed once, false otherwise
   */
  bool keyPressedOnce(int keyCode);

  /**
   * Tell the app to closes its window and releases all resources.
   * Also sets the error flag if an error has occured.
   * @param errorHasOccured Should be true, if an error has occured (default is
   * false).
   */
  void closeWindow(bool errorHasOccured = false);

  /**
   * Gets the error flag.
   */
  bool hasErrorOccurred() const;

  /**
   * Gets the projection matrix for the current window size.
   */
  glm::mat4 getProjectionMatrix() const;

  /**
   * Gets the adjusted float value, that takes frames per second into account.
   * @param value Float value to be adjusted
   * @return The Speed Adjusted Float (saf)".
   */
  float saf(float value) const;

  /**
   * Gets time delta (time passed since the last frame, in seconds).
   */
  double getTimeDelta() const;

  /**
   * Gets current FPS (Frames Per Second).
   */
  int getFPS() const;

  /**
   * Turns vertical synchronization on or off.
   * @param enable True if you want to enable VSync, false otherwise
   */
  void setVerticalSync(bool enable);

  /**
   * Checks if vertical synchronization is on or off.
   *
   * @return True if VSync is enabled, false otherwise
   */
  bool isVerticalSyncEnabled() const;

  /**
   * Gets the window's current  width (in pixels).
   */
  int getWindowWidth() const;

  /**
   * Gets the window's current height (in pixels).
   */
  int getWindowHeight() const;

  /**
   * Gets the cursor's current position (in pixels).
   */
  glm::ivec2 getCursorPosition() const;

  /**
   * Gets the window's current size (in pixels).
   */
  glm::ivec2 getWindowSize() const;

 private:
  GLFWwindow* _window = nullptr;  // Pointer to GLFWwindow, nullptr by default
  bool _keyWasPressed[512];  // Array of bools (used by keyPressedOnce function)
  bool _hasErrorOccurred = false;  // Error flag

  glm::mat4 _projectionMatrix;  // Precalculated projection matrix, when size
                                // changes, it's recalculated

  double _lastFrameTime = 0.0;     // Time of last frame
  double _lastFrameTimeFPS = 0.0;  // Time of last FPS Update
  double _timeDelta = 0.0;  // Time delta between last frame and current frame
  int _FPS = 0;             // Current FPS
  int _nextFPS = 0;  // Next FPS, that is being counted and will update the _FPS

  bool _isVerticalSyncEnabled = false;  // Stores whether VSync is enabled

  int _minWindowWidth = 800;   // Minimum window width
  int _minWindowHeight = 600;  // Minimum window height
  int _windowWidth = 0;        // Cached window width
  int _windowHeight = 0;       // Cached window height

  /**
   * Update the window title with new information
   * @param baseTitle Start of the window title
   * @param cameraPos Positon of the camera
   * @param separator Separator between informations
   */
  void _updateWindowTitle(const std::string& baseTitle,
                          const glm::vec3& cameraPos,
                          const std::string& separator = " | ");

  /**
   * Recalculates the app's projection matrix
   */
  void _recalculateProjectionMatrix();

  /**
   * Updates frame times and FPS counters.
   */
  void _updateDeltaTimeAndFPS();

  /**
   * Gets called when window size is changed. Does some internal stuff
   * (like recalculating matrices, setting viewport) and calls user's handler as
   * well.
   */
  void _onWindowResizeInternal(int width, int height);

  /**
   * Get the pointer to the app associated with a GLFW window
   * @param window Pointer to the GLFW Window
   * @return Pointer to the App associated with the window
   */
  static App* _getAppPtrFromWindow(GLFWwindow* window);

  /**
   * Static method that is set as callback to GLFW framework about window size
   * changed.
   */
  static void _onWindowResizeStatic(GLFWwindow* window, int width, int height);
};

#endif