#include "vertex.hpp"

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv)
    : position(position), normal(normal), uv(uv) {}

Vertex2dColor::Vertex2dColor(glm::vec2 position, glm::vec3 color)
    : position(position), color(color) {}

Vertex2dUV::Vertex2dUV(glm::vec2 position, glm::vec2 uv)
    : position(position), uv(uv) {}

Vertex3dColor::Vertex3dColor(glm::vec3 position, glm::vec3 color)
    : position(position), color(color) {}

Vertex3dUV::Vertex3dUV(glm::vec3 position, glm::vec2 uv)
    : position(position), uv(uv) {}
