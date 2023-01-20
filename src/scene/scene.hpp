#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "../shader_structs/ambient_light.hpp"
#include "../shader_structs/directional_light.hpp"
#include "../shader_structs/material.hpp"
#include "../shader_structs/point_light.hpp"

#include "globject.hpp"

class Scene {
 public:
  glm::vec4 backgroundColor;

  // Objects
  std::vector<std::unique_ptr<GLObject>> objects;

  // Ambient lights
  static const size_t MAX_AMBIENT_LIGHTS = 8;  // Same as in shaders
  std::vector<shader_structs::AmbientLight> ambientLights;

  // Directional lights
  static const size_t MAX_DIRECTIONAL_LIGHTS = 8;  // Same as in shaders
  std::vector<shader_structs::DirectionalLight> directionalLights;

  // Point lights
  static const size_t MAX_POINT_LIGHTS = 8;  // Same as in shaders
  std::vector<shader_structs::PointLight> pointLights;

  Scene(const glm::vec4& backgroundColor = glm::vec4(0),
        const bool isDefault = false);

 private:
  void _initDefaultScene();
};

#endif