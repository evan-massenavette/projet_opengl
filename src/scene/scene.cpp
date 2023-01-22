#include <iostream>
#include <utility>

#include "scene.hpp"

Scene::Scene(const glm::vec4& backgroundColor, const bool isDefault)
    : backgroundColor(backgroundColor) {
  if (isDefault)
    _initDefaultScene();
}

void Scene::_initDefaultScene() {
  // Objects
  auto object = new GLObject("cube");
  objects.emplace_back(object);

  // Ambient lights
  shader_structs::AmbientLight ambientLight(glm::vec3(1, 1, 1), 0.1f);
  ambientLights.emplace_back(ambientLight);

  // Directional lights
  shader_structs::DirectionalLight directionalLight1(glm::vec3(0, 1, 0), 1,
                                                     glm::vec3(-1, -1, 1));
  // directionalLights.emplace_back(directionalLight1);

  // Point lights
  shader_structs::PointLight pointLight1(glm::vec3(1, 0, 0), 1,
                                         glm::vec3(5, 5, 5), 0.01);
  pointLights.emplace_back(pointLight1);
}