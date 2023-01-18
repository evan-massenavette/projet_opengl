#include "directional_light.hpp"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color)
    : direction(direction), color(color) {}