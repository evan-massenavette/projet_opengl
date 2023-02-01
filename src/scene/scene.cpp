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
      backgroundColor(glm::vec4(colors_utils::skyBlue, 1))
{
  if (isDefault)
    _initDefaultScene();
}

void Scene::_initDefaultScene()
{
  // Objects
  auto cart = new SceneObject("cart");
  auto coaster = new SceneObject("coaster");
  auto tree = new SceneObject("tree_1");
  auto building1 = new SceneObject("building_1");
  auto building2 = new SceneObject("building_2");
  auto lamp = new SceneObject("lamp_post");
  auto lantern = new SceneObject("lantern");
  auto rock_1 = new SceneObject("rock");
  auto rock_2 = new SceneObject("rock");
  auto tree1 = new SceneObject("tree_1");
  auto tree2 = new SceneObject("tree_2");
  auto tree3 = new SceneObject("tree_3");
  auto tree4_1 = new SceneObject("tree_4");
  auto tree4_2 = new SceneObject("tree_4");
  auto tree4_3 = new SceneObject("tree_4");
  auto tree5 = new SceneObject("tree_5");

  coaster->setPosition(glm::vec3(0.0, 0.0, 0.0));
  tree->setPosition(glm::vec3(3.0, 0.0, -20.0));
  cart->setPosition(glm::vec3(-25.204239, 9.094718, -24.467152));
  lamp->setPosition(glm::vec3(20.0, 0.0, 20.0));
  lantern->setPosition(glm::vec3(30.0, 1.65, -18.0));
  rock_1->setPosition(glm::vec3(40.0, 0.0, -20.0));
  rock_2->setPosition(glm::vec3(0.0, 9.0, 0.0));
  building1->setPosition(glm::vec3(25.0, 0.0, -60.0));
  building2->setPosition(glm::vec3(-80.0, 0.0, 0.0));
  tree4_1->setPosition(glm::vec3(-60, 0, 60));
  tree4_2->setPosition(glm::vec3(0, 0, 70));
  tree4_3->setPosition(glm::vec3(0, 0, -70));
  tree1->setPosition(glm::vec3(-20, 7, 0));
  tree2->setPosition(glm::vec3(-20, 0, 20));
  tree3->setPosition(glm::vec3(20, 0, -20));
  tree5->setPosition(glm::vec3(-60, 0, 60));

  cart->setScale(glm::vec3(3.0));
  lantern->setScale(glm::vec3(0.01));
  building1->setScale(glm::vec3(0.10));
  building2->setScale(glm::vec3(0.10));
  tree3->setScale(glm::vec3(0.4));
  lamp->setScale(glm::vec3(0.5));
  rock_2->setScale(glm::vec3(1.5));

  building2->setRotation(glm::vec3(0.0, 1.57, 0.0));
  tree4_2->setRotation(glm::vec3(0.0, 1.57, 0.0));
  tree4_3->setRotation(glm::vec3(0.0, 3.14, 0.0));
  rock_2->setRotation(glm::vec3(3.14, 0.0, 0.0));

  objects.emplace_back(cart);
  objects.emplace_back(coaster);
  objects.emplace_back(tree);
  objects.emplace_back(building1);
  objects.emplace_back(building2);
  objects.emplace_back(lamp);
  objects.emplace_back(lantern);
  objects.emplace_back(rock_1);
  objects.emplace_back(rock_2);
  objects.emplace_back(tree1);
  objects.emplace_back(tree2);
  objects.emplace_back(tree3);
  objects.emplace_back(tree4_1);
  objects.emplace_back(tree4_2);
  objects.emplace_back(tree4_3);
  objects.emplace_back(tree5);

  // Ambient lights
  shader_structs::AmbientLight ambientLight(glm::vec3(1.0, 1.0, 1.0), 0.1f);
  ambientLights.emplace_back(ambientLight);

  // Directional lights
  shader_structs::DirectionalLight directionalLight1(
      glm::vec3(1.0, 1.0, 1.0), 0.5f, glm::vec3(-1.0, -1.0, 1.0));
  directionalLights.emplace_back(directionalLight1);

  // Point lights
  shader_structs::PointLight pointLight1(glm::vec3(1.0, 1.0, 0.8), 1.0f,
                                         glm::vec3(30.0, 2.0, -18.0), 1e-3f);
  shader_structs::PointLight pointLight2(glm::vec3(1.0, 1.0, 0.8), 1.0f,
                                         glm::vec3(22.0, 9.0, 20.0), 1e-4f);
  pointLights.emplace_back(pointLight1);
  pointLights.emplace_back(pointLight2);
}

void Scene::update(const std::function<float(float)> &speedCorrectionFunc)
{
  // If first update, initialize the cart
  if (_cart.needsInit && !spline::cart.empty())
  {
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
  glm::vec3 positionOffset(0, 2, 0);

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
  auto &cart = objects.front();
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
