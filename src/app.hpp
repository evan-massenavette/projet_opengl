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
   *
   *  @param windowTitle     Title of the created window
   *  @param majorVersion    OpenGL context major version
   *  @param minorVersion    OpenGL context minor version
   *  @param showFullscreen  Tells, whether the window should be shown in
   * fullscreen
   *
   *  @return True if window has been created successfully or false otherwise.
   */
  bool createWindow(const std::string& windowTitle,
                    int majorVersion,
                    int minorVersion,
                    bool showFullscreen);

  void destroyWindow();

  /**
   * Creates a window with OpenGL context with given title and context version.
   *
   * @param windowTitle  Title of a created window
   *
   * @return Pointer to GLFW window or nullptr, if the window does not exist
   * yet.
   */
  GLFWwindow* getWindow() const;

  /**
   * Runs the whole application. Contains the main application loop.
   */
  void run();

  /**
   * Checks, if specified key is currently pressed.
   *
   * @param keyCode  GLFW code of the key to check
   *
   * @return True if key is pressed or false otherwise.
   */
  bool keyPressed(int keyCode) const;

  /**
   * Checks, if specified key was pressed. This function won't return true
   * again, unless the key has been released and pressed again.
   *
   * @param keyCode  GLFW code of the key to check
   *
   * @return True, if key was pressed once, or false otherwise.
   */
  bool keyPressedOnce(int keyCode);

  /**
   * Closes application window and releases all resources. Also sets the error
   * flag, if error has occured.
   *
   * @param errorHasOccured  Should be true, if an error has occured (default is
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
   * Gets adjusted float value, that takes frames per second into account.
   *
   * @param value  Float value to be adjusted
   *
   * @return "Speed Optimized Floating Point Value (sof)".
   */
  float sof(float value) const;

  /**
   * Gets time delta - time passed since the last frame in seconds.
   */
  double getTimeDelta() const;

  /**
   * Gets current Frames Per Second (FPS).
   */
  int getFPS() const;

  /**
   * Turns vertical synchronization on or off.
   *
   * @param enable  True if you want to enable vertical synchronization or false
   * otherwise
   */
  void setVerticalSynchronization(bool enable);

  /**
   * Checks, if vertical synchronization is on or off.
   *
   * @return True, if vertical synchronization is enabled, or false otherwise.
   */
  bool isVerticalSynchronizationEnabled() const;

  /**
   * Gets current screen width in pixels.
   */
  int getScreenWidth() const;

  /**
   * Gets current screen height in pixels.
   */
  int getScreenHeight() const;

  /**
   * Gets cursor position in OpenGL coordinates.
   */
  glm::ivec2 getCursorPosition() const;

 private:
  GLFWwindow* _window = nullptr;  // Pointer to GLFWwindow, nullptr by default
  bool _keyWasPressed[512];  // Array of bools, used by keyPressedOnce function
  bool hasErrorOccurred_ =
      false;  // Error flag, indicates, if any kind of error has occured

  glm::mat4 _projectionMatrix;  // Precalculated projection matrix, when size
                                // changes, it's recalculated

  double _lastFrameTime = 0.0;     // Time of last frame
  double _lastFrameTimeFPS = 0.0;  // Time of last FPS Update
  double _timeDelta = 0.0;  // Time delta between last frame and current frame
  int _FPS = 0;             // Current FPS
  int _nextFPS = 0;  // Next FPS, that is being counted and will update the _FPS
  bool _isVerticalSynchronizationEnabled =
      false;  // Stores information, if vertical synchronization is enabled

  bool _hasMouseBeenScrolled = false;
  double _scrollOffsetX = 0.0;
  double _scrollOffsetY = 0.0;

  int minScreenWidth_ = 800;   // Minimum screen width
  int minScreenHeight_ = 600;  // Minimum screen height
  int screenWidth_ = 0;        // Cached screen width
  int screenHeight_ = 0;       // Cached screen height

  /**
   * Recalculates the app's projection matrix
   */
  void recalculateProjectionMatrix();

  /**
   * Updates frame times and FPS counters.
   */
  void updateDeltaTimeAndFPS();

  /**
   * Gets called when window size is changed. Does some internal stuff
   * (like recalculating matrices, setting viewport) and calls user's handler as
   * well.
   */
  void onWindowSizeChangedInternal(int width, int height);

  /**
   * Get the pointer to the app associated with a GLFW window
   * @param window Pointer to the GLFW Window
   * @return Pointer to the App associated with the window
   */
  static App* getAppPtrFromWindow(GLFWwindow* window);

  /**
   * Static method that is set as callback to GLFW framework about window size
   * changed.
   */
  static void onWindowSizeChangedStatic(GLFWwindow* window,
                                        int width,
                                        int height);
};

#endif