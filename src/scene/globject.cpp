#include <iostream>
#include <memory>
#include <string>
#include <utility>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "vertex.hpp"

#include "globject.hpp"

GLObject::GLObject(const std::string &modelName)
{
  _loadModel(modelName);
  _bufferData();
}

GLObject::~GLObject()
{
  // Cleanup
  objectMaterials.clear();
}

void GLObject::_loadModel(const std::string &modelName)
{
  printf("Loading model: %s\n", modelName.c_str());

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
                              ("models/" + modelName + "/model.obj").c_str(),
                              ("models/" + modelName + "/").c_str());

  if (!err.empty())
  {
    std::cerr << err << std::endl;
  }

  if (!ret)
  {
    exit(1);
  }

  // Load object materials
  for (tinyobj::material_t &material : materials)
  {
    // Load material elements
    glm::vec3 ambient(material.ambient[0], material.ambient[1], material.ambient[2]);
    glm::vec3 diffuse(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
    glm::vec3 specular(material.specular[0], material.specular[1], material.specular[2]);
    float shininess = material.shininess;
    shader_structs::Material modelMaterial(ambient, diffuse, specular, shininess);
    auto objectMaterial = new ObjectMaterial(modelMaterial);

    // Load texture
    std::string textureFilename = material.diffuse_texname;
    auto texture = std::make_shared<Texture>();
    texture->loadTexture2D("models/" + modelName + "/textures/" + textureFilename);
    objectMaterial->texture = texture;

    objectMaterials.emplace_back(objectMaterial);
  }

  printf("Vertices: %llu\n", attrib.vertices.size());
  printf("Normals: %llu\n", attrib.normals.size());
  printf("Texture Coords: %llu\n", attrib.texcoords.size());

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++)
  {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
    {
      // per-face material
      int idx_material = shapes[s].mesh.material_ids[f];

      size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

      // Loop over vertices in the face.
      for (size_t v = 0; v < fv; v++)
      {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

        tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
        tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
        tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

        glm::vec3 position(vx, vy, vz);

        // Check if `normal_index` is zero or positive. negative = no normal
        // data
        glm::vec3 normal;
        if (idx.normal_index >= 0 && attrib.normals.size() > 0)
        {
          tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
          tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
          tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
          normal = glm::vec3(nx, ny, nz);
        }
        else
        {
          normal = glm::vec3(0);
        }

        // Check if `texcoord_index` is zero or positive. negative = no texcoord
        // data
        glm::vec2 uv;
        if (idx.texcoord_index >= 0 && attrib.texcoords.size() > 0)
        {
          tinyobj::real_t tx =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
          tinyobj::real_t ty =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
          uv = glm::vec2(tx, ty);
        }
        else
        {
          uv = glm::vec2(-1);
        }

        // Optional: vertex colors
        // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
        // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
        // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

        Vertex vertex(position, normal, uv);
        objectMaterials[idx_material]->vertices.push_back(vertex);
      }
      index_offset += fv;
    }
  }
}

void GLObject::draw(Uniform textureSampler)
{
  for (auto &objectMaterial : objectMaterials)
  {
    objectMaterial->draw(textureSampler);
  }
}

void GLObject::_bufferData()
{
  for (auto &objectMaterial : objectMaterials)
  {
    objectMaterial->bufferData();
  }
}