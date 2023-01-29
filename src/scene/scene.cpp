#include <iostream>
#include <utility>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "scene.hpp"
#include "../spline.hpp"

Scene::Scene(const glm::vec4 &backgroundColor, const bool isDefault)
    : backgroundColor(backgroundColor)
{
  if (isDefault)
    _initDefaultScene();
}

void Scene::_initDefaultScene()
{
  // Objects
  auto object1 = new SceneObject("coaster");
  auto object2 = new SceneObject("tree");
  auto object3 = new SceneObject("cart");
  object1->setPosition(glm::vec3(0.0, 0.0, 0.0));
  object2->setPosition(glm::vec3(3.0, 12.0, -20.0));
  object3->setScale(glm::vec3(3, 3, 3));
  object3->setPosition(glm::vec3(0, 5, -8));

  objects.emplace_back(object1);
  objects.emplace_back(object2);
  objects.emplace_back(object3);

  // Ambient lights
  shader_structs::AmbientLight ambientLight(glm::vec3(1.0, 1.0, 1.0), 0.1f);
  ambientLights.emplace_back(ambientLight);

  // Directional lights
  shader_structs::DirectionalLight directionalLight1(glm::vec3(1.0, 1.0, 1.0), 0.5f,
                                                     glm::vec3(-1.0, -1.0, 1.0));
  directionalLights.emplace_back(directionalLight1);

  // Point lights
  shader_structs::PointLight pointLight1(glm::vec3(1.0, 1.0, 1.0), 1.0f,
                                         glm::vec3(5.0, 5.0, 5.0), 1e-3f);
  // pointLights.emplace_back(pointLight1);
}

void Scene::update()
{
  // objects[2]->setPosition(spline[_t % 60] + glm::vec3(0, 5, 0));
  //_t++;
}