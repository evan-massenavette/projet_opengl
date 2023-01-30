#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtx/vector_angle.hpp>

#include "../spline.hpp"
#include "../utils/colors_utils.hpp"
#include "../utils/math_utils.h"
#include "../utils/string_utils.hpp"

#include "scene.hpp"

Scene::Scene(const bool isDefault)
    : fogParams(colors_utils::skyBlue, 0.015f),
      backgroundColor(glm::vec4(colors_utils::skyBlue, 1)) {
  if (isDefault)
    _initDefaultScene();
}

void Scene::_initDefaultScene() {
  // Objects
  auto cart = new SceneObject("cart");
  auto coaster = new SceneObject("coaster");
  auto tree = new SceneObject("tree");
  coaster->setPosition(glm::vec3(0.0, 0.0, 0.0));
  tree->setPosition(glm::vec3(3.0, 12.0, -20.0));
  cart->setScale(glm::vec3(3, 3, 3));
  cart->setPosition(glm::vec3(0.490937, 16.066832, -27.641771));

  objects.emplace_back(cart);
  objects.emplace_back(coaster);
  objects.emplace_back(tree);

  // Ambient lights
  shader_structs::AmbientLight ambientLight(glm::vec3(1.0, 1.0, 1.0), 0.1f);
  ambientLights.emplace_back(ambientLight);

  // Directional lights
  shader_structs::DirectionalLight directionalLight1(
      glm::vec3(1.0, 1.0, 1.0), 0.5f, glm::vec3(-1.0, -1.0, 1.0));
  directionalLights.emplace_back(directionalLight1);

  // Point lights
  shader_structs::PointLight pointLight1(glm::vec3(1.0, 1.0, 1.0), 1.0f,
                                         glm::vec3(5.0, 5.0, 5.0), 1e-3f);
  // pointLights.emplace_back(pointLight1);
}

void Scene::update(const std::function<float(float)>& speedCorrectionFunc) {
  // If first update, initialize the cart
  if (_cart.needsInit && !spline::cart.empty()) {
    _cart.lastPosition = spline::cart.front();
    _cart.needsInit = false;
    return;
  }

  // Get the current index in the cart spline
  double totalCycleTime = 10;
  double currentCycleTime = std::fmod(glfwGetTime(), totalCycleTime);

  // Calculate indices
  size_t index = static_cast<size_t>(_cart.realIndex);
  size_t nextIndex = math_utils::pos_mod(index + 1, spline::cart.size());

  // Cart position (interpolated, for smooth movement)
  float interp = static_cast<float>(_cart.realIndex - index);
  glm::vec3 position =
      (1 - interp) * spline::cart[index] + interp * spline::cart[nextIndex];
  glm::vec3 positionOffset(0, 4, 0);

  // Movement vectors
  glm::vec3 movement = position - _cart.lastPosition;
  glm::vec3 normalizedMovement = glm::normalize(movement);

  // World axes
  glm::vec3 xAxis(1, 0, 0);
  glm::vec3 yAxis(0, 1, 0);
  glm::vec3 zAxis(0, 0, 1);

  // Rotation angles
  float yAngle = -glm::orientedAngle(normalizedMovement, xAxis, yAxis);
  float zAngle = static_cast<float>(math_utils::PI / 2) -
                 glm::angle(normalizedMovement, yAxis);

  // Set the cart's position and rotation
  auto& cart = objects.front();
  cart->setPosition(position + positionOffset);
  cart->setRotation(glm::vec3(0, yAngle, zAngle));

  // Update member vars
  _cart.lastPosition = position;
  _cart.acceleration = -movement.y * _cart.gravity * _cart.weight;
  _cart.speed =
      std::clamp(_cart.speed + speedCorrectionFunc(_cart.acceleration),
                 _cart.minSpeed, _cart.maxSpeed);
  _cart.realIndex =
      math_utils::pos_fmod(_cart.realIndex + speedCorrectionFunc(_cart.speed),
                           static_cast<float>(spline::cart.size()));
}
