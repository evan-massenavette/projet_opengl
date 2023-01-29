#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "app.hpp"
#include "camera/camera.hpp"
#include "camera/flying_camera.hpp"
#include "camera/following_camera.hpp"

#include <glm/glm.hpp>

/**
 * Handles controls of the application
 */
class Controls {
 public:
  // Constructor
  Controls(Camera::Type cameraType = Camera::Type::Flying);

  /**
   * Processes the inputs received
   */
  void processInputs(App& app);

  Camera& getCurrentCamera(FlyingCamera& flyingCamera,
                           FollowingCamera& followingCamera);

 private:
  Camera::Type _cameraType;
};

#endif