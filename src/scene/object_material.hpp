#ifndef OBJECT_MATERIAL_HPP
#define OBJECT_MATERIAL

#include <memory>
#include <vector>

#include <glad/glad.h>

#include "../gl_wrappers/texture.hpp"
#include "../gl_wrappers/vertex_buffer_object.hpp"
#include "../shader_structs/material.hpp"
#include "vertex.hpp"

class ObjectMaterial
{
public:
     VertexBufferObject vbo;
     GLuint vao;
     std::vector<Vertex> vertices;
     std::shared_ptr<Texture> texture;
     shader_structs::Material material;

     ObjectMaterial(shader_structs::Material material);
     ~ObjectMaterial();

     // Disable copy constructor
     ObjectMaterial(const ObjectMaterial &) = delete;
     ObjectMaterial &operator=(const ObjectMaterial &) = delete;

     void bufferData();
     void draw(Uniform textureSampler);
};
#endif