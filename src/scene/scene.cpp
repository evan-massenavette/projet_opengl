#include <cmath>
#include <iostream>
#include <utility>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtx/vector_angle.hpp>

#include "../spline.hpp"
#include "../utils/colors.hpp"

#include "scene.hpp"

constexpr double PI = 3.1415926535897932384626433832795028841971693993751058209;

Scene::Scene(const bool isDefault)
    : fogParams(colors::skyBlue, 0.015f),
      backgroundColor(glm::vec4(colors::skyBlue, 1)) {
  if (isDefault)
    _initDefaultScene();
}

void Scene::_initDefaultScene() {
  // Objects
  auto object1 = new SceneObject("coaster");
  auto object2 = new SceneObject("tree");
  auto object3 = new SceneObject("cart");
  object1->setPosition(glm::vec3(0.0, 0.0, 0.0));
  object2->setPosition(glm::vec3(3.0, 12.0, -20.0));
  object3->setScale(glm::vec3(3, 3, 3));
  object3->setPosition(glm::vec3(0.490937, 16.066832, -27.641771));

  objects.emplace_back(object1);
  objects.emplace_back(object2);
  objects.emplace_back(object3);

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

void Scene::update() {
  // Get the current index in the cart spline
  double totalCycleTime = 10;
  double currentCycleTime = std::fmod(glfwGetTime(), totalCycleTime);

  // Calculate indices
  double realIndex = currentCycleTime / totalCycleTime * spline::cart.size();
  size_t index = static_cast<size_t>(realIndex);
  size_t nextIndex = index < spline::cart.size() - 1 ? index + 1 : 0;

  // Cart position (interpolated, for smooth movement)
  float interp = static_cast<float>(realIndex - index);
  glm::vec3 position =
      (1 - interp) * spline::cart[index] + interp * spline::cart[nextIndex];
  glm::vec3 positionOffset(0, 4, 0);

  // Movement vectors
  glm::vec3 movement = position - _lastCartPosition;
  glm::vec3 normalizedMovement = glm::normalize(movement);

  // World axes
  glm::vec3 xAxis(1, 0, 0);
  glm::vec3 yAxis(0, 1, 0);
  glm::vec3 zAxis(0, 0, 1);

  // Rotation angles
  glm::vec3 xzMovement =
      glm::dot(movement, xAxis) * xAxis + glm::dot(movement, zAxis) * zAxis;
  glm::vec3 xzNormalizedMovement = glm::normalize(xzMovement);
  float xAngle = glm::angle(normalizedMovement, xzNormalizedMovement);
  float yAngle = -glm::orientedAngle(normalizedMovement, xAxis, yAxis);

  // Set the cart's position and rotation
  auto& cart = objects.back();
  cart->setPosition(position + positionOffset);
  cart->setRotation(glm::vec3(xAngle, yAngle, 0));

  // Update member vars
  _lastCartPosition = position;
}