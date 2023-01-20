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
  glm::vec3 direction1(1, 1, 1);
  glm::vec3 color1(1, 0, 0);
  float factor1 = 1;
  shader_structs::DirectionalLight directionalLight1(direction1, color1,
                                                     factor1);

  glm::vec3 direction2(1, 0, 1);
  glm::vec3 color2(0, 1, 0);
  float factor2 = 1;
  shader_structs::DirectionalLight directionalLight2(direction2, color2,
                                                     factor2);

  directionalLights.emplace_back(directionalLight1);
  directionalLights.emplace_back(directionalLight2);
}