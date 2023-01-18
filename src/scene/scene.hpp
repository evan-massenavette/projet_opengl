#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <vector>

#include "../lighting/directional_light.hpp"
#include "globject.hpp"

class Scene {
public:
  std::vector<std::unique_ptr<GLObject>> objects;

  static constexpr size_t MAX_NUM_DIRECTIONAL_LIGHTS = 8;
  std::vector<DirectionalLight> directionalLights;

  Scene();
};

#endif