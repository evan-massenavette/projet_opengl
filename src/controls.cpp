#include "keybinds.hpp"

#include "controls.hpp"

void Controls::processInputs(GLFWwindow* window) {
  // Exit the app
Camera& Controls::getCurrentCamera(FlyingCamera& flyingCamera,
                                   FollowingCamera& followingCamera) {
  switch (_cameraType) {
    case Camera::Type::Flying:
      return flyingCamera;
    case Camera::Type::Following:
      return followingCamera;
    default:
      throw std::runtime_error("Unkown camera type");
  }
}
