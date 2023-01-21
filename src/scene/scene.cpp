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

  // Ambient lights
  shader_structs::AmbientLight ambientLight(glm::vec3(1, 1, 1), 0.1f);
  ambientLights.emplace_back(ambientLight);

  // Directional lights
  shader_structs::DirectionalLight directionalLight1(glm::vec3(1, 1, 1),
                                                     glm::vec3(0, 1, 0), 1);
  // directionalLights.emplace_back(directionalLight1);

  // Point lights
  shader_structs::PointLight pointLight1(glm::vec3(1.5, 1.5, 1.5),
                                         glm::vec3(1, 0, 0), 1);
  pointLights.emplace_back(pointLight1);
}