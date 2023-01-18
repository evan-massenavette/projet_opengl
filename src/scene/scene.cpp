#include <utility>

#include "scene.hpp"

Scene::Scene() {
  // Add object
  auto object = new GLObject("models/cube/cube.obj",
                             "models/cube/textures/cube_albedo.bmp");
  objects.emplace_back(object);

  // Add light source
  glm::vec3 direction(1, 1, 1);
  glm::vec3 color(1, 0, 0);
  DirectionalLight directionalLight(direction, color);
  directionalLights.emplace_back(directionalLight);
}