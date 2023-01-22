#include <iostream>
#include <string>
#include <utility>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "../vertex.hpp"

#include "globject.hpp"

GLObject::GLObject(const std::string& modelFilepath) {
  _loadModel(modelFilepath);
}

GLObject::~GLObject() {
  // Cleanup
  faces.clear();
}

void GLObject::_loadModel(const std::string& modelFilepath) {
  printf("Loading model: %s\n", modelFilepath.c_str());

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;

  bool ret = tinyobj::LoadObj(
      &attrib, &shapes, &materials, &err,
      ("models/" + modelFilepath + "/" + modelFilepath + ".obj").c_str(),
      ("models/" + modelFilepath + "/").c_str());

  if (!err.empty()) {
    std::cerr << err << std::endl;
  }

  if (!ret) {
    exit(1);
  }

  printf("Vertices: %llu\n", attrib.vertices.size());
  printf("Normals: %llu\n", attrib.normals.size());
  printf("Texture Coords: %llu\n", attrib.texcoords.size());

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      std::vector<Vertex> vertices;

      size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

      // Loop over vertices in the face.
      for (size_t v = 0; v < fv; v++) {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

        tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
        tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
        tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

        glm::vec3 position(vx, vy, vz);

        // Check if `normal_index` is zero or positive. negative = no normal
        // data
        glm::vec3 normal;
        if (idx.normal_index >= 0 && attrib.normals.size() > 0) {
          tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
          tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
          tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
          normal = glm::vec3(nx, ny, nz);
        } else {
          normal = glm::vec3(0);
        }

        // Check if `texcoord_index` is zero or positive. negative = no texcoord
        // data
        glm::vec2 uv;
        if (idx.texcoord_index >= 0 && attrib.texcoords.size() > 0) {
          tinyobj::real_t tx =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
          tinyobj::real_t ty =
              attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
          uv = glm::vec2(tx, ty);
        } else {
          uv = glm::vec2(-1);
        }

        // Optional: vertex colors
        // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
        // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
        // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

        Vertex vertex(position, normal, uv);
        vertices.push_back(vertex);
      }
      index_offset += fv;

      // per-face material
      int idx_material = shapes[s].mesh.material_ids[f];
      tinyobj::material_t material_face = materials[idx_material];
      glm::vec3 ambient(material_face.ambient[0], material_face.ambient[1],
                        material_face.ambient[2]);
      glm::vec3 diffuse(material_face.diffuse[0], material_face.diffuse[1],
                        material_face.diffuse[2]);
      glm::vec3 specular(material_face.specular[0], material_face.specular[1],
                         material_face.specular[2]);
      float shininess = material_face.shininess;
      std::string textureFilepath = material_face.diffuse_texname;
      shader_structs::Material material(ambient, diffuse, specular, shininess);

      // Load texture
      GLuint texture = Texture::load("models/" + modelFilepath + "/textures/" +
                                     textureFilepath);

      // Add to the faces
      auto current_face = new Face(vertices, texture, material);
      current_face->bufferData();
      faces.emplace_back(current_face);
    }
  }
}

void GLObject::draw(Uniform textureSampler) {
  for (auto& face : faces) {
    face->draw(textureSampler);
  }
}
