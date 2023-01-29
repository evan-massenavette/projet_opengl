#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "../shader_structs/ambient_light.hpp"
#include "../shader_structs/directional_light.hpp"
#include "../shader_structs/fog_parameters.hpp"
#include "../shader_structs/material.hpp"
#include "../shader_structs/point_light.hpp"
#include "scene_object.hpp"

class Scene {
 public:
  glm::vec4 backgroundColor;

  // Objects
  std::vector<std::unique_ptr<SceneObject>> objects;

  // Ambient lights
  static const size_t MAX_AMBIENT_LIGHTS = 8;  // Same as in shaders
  std::vector<shader_structs::AmbientLight> ambientLights;

  // Directional lights
  static const size_t MAX_DIRECTIONAL_LIGHTS = 8;  // Same as in shaders
  std::vector<shader_structs::DirectionalLight> directionalLights;

  // Point lights
  static const size_t MAX_POINT_LIGHTS = 8;  // Same as in shaders
  std::vector<shader_structs::PointLight> pointLights;

  shader_structs::FogParameters fogParams;

  Scene(const bool isDefault = false);

  void update(float timeDelta);

 private:
  struct Cart {
    glm::vec3 lastPosition;
    float minSpeed = 0.5;
    float maxSpeed = 4;
    float speed = 0.5;
    float acceleration = 0;
    float gravity = 9.81f;
    float weight = 5;
    float realIndex = 0;
    bool needsInit = true;
  };

  Cart _cart;
  void _initDefaultScene();
};

#endif