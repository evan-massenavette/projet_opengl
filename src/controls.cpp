#include <stdexcept>

#include <glm/glm.hpp>

#include "keybinds.hpp"

#include "controls.hpp"

Controls::Controls(Camera::Type cameraType) : _cameraType(cameraType) {}

void Controls::processInputs(App& app) {
  // Exit the app
  if (app.keyPressed(Keybinds::exitApp)) {
    app.closeWindow();
  }

  // Change type of camera
  if (app.keyPressedOnce(Keybinds::changeCameraType)) {
    switch (_cameraType) {
      case Camera::Type::Flying:
        _cameraType = Camera::Type::Following;
        break;
      case Camera::Type::Following:
        _cameraType = Camera::Type::Flying;
        break;
      default:
        throw std::runtime_error("Unkown camera type");
    }
  }
}

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
