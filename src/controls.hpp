#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Controls {
 public:
  // Constructor
  Controls(Camera::Type cameraType = Camera::Type::Flying);
  Camera& getCurrentCamera(FlyingCamera& flyingCamera,
                           FollowingCamera& followingCamera);

 private:
  Camera::Type _cameraType;
};

#endif