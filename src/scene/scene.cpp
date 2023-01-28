#include <iostream>
#include <utility>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "scene.hpp"

Scene::Scene(const glm::vec4& backgroundColor, const bool isDefault)
    : backgroundColor(backgroundColor) {
  if (isDefault)
    _initDefaultScene();
}

void Scene::_initDefaultScene() {
  // Objects
  auto object1 = new SceneObject("coaster");
  auto object2 = new SceneObject("tree");
  auto object3 = new SceneObject("cart");

  objects.emplace_back(object1);
  objects.emplace_back(object2);
  objects.emplace_back(object3);

  // Ambient lights
  shader_structs::AmbientLight ambientLight(glm::vec3(1, 1, 1), 0.1f);
  ambientLights.emplace_back(ambientLight);

  // Directional lights
  shader_structs::DirectionalLight directionalLight1(glm::vec3(1, 1, 1), 0.5,
                                                     glm::vec3(-1, -1, 1));
  directionalLights.emplace_back(directionalLight1);

  // Point lights
  shader_structs::PointLight pointLight1(glm::vec3(1, 1, 1), 1,
                                         glm::vec3(5, 5, 5), 1e-3);
  // pointLights.emplace_back(pointLight1);
}

void Scene::update() {
  auto& cart = objects.back();

  auto cartPos =
      glm::vec3(0, 10, 5) + glm::vec3(glm::sin(glfwGetTime()) * 5, 0, 0);
  cart->setPosition(cartPos);
}