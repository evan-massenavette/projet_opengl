#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "../shader_structs/directional_light.hpp"

#include "globject.hpp"

class Scene {
 public:
  glm::vec4 backgroundColor;

  // Objects
  std::vector<std::unique_ptr<GLObject>> objects;

  // Directional lights
  static const size_t MAX_DIRECTIONAL_LIGHTS = 8;
  std::vector<shader_structs::DirectionalLight> directionalLights;

  Scene(bool isDefault = false);
  Scene(glm::vec4 backgroundColor, bool isDefault);

 private:
  void _initDefaultScene();
};

#endif