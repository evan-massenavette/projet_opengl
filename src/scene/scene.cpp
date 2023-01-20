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
  auto object = new GLObject("models/cube/cube.obj",
                             "models/cube/textures/cube_albedo.bmp");
  objects.emplace_back(object);

  // Directional lights
  shader_structs::DirectionalLight directionalLight1(glm::vec3(1, 1, 1),
                                                     glm::vec3(1, 0, 0), 1);

  shader_structs::DirectionalLight directionalLight2(glm::vec3(1, 0, 1),
                                                     glm::vec3(0, 1, 0), 1);

  directionalLights.emplace_back(directionalLight1);
  directionalLights.emplace_back(directionalLight2);
}