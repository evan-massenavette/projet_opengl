#include <iostream>
#include <string>
#include <utility>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "../vertex.hpp"

#include "globject.hpp"

GLObject::GLObject(const char* modelFilepath, const char* textureFilepath) {
  _loadModel(modelFilepath, textureFilepath);
  _bufferData();
}

GLObject::~GLObject() {
  // Cleanup
  vbo.deleteVBO();
  glDeleteVertexArrays(1, &vao);
}

void GLObject::_loadModel(const char* modelFilepath,
                          const char* textureFilepath) {
  printf("Loading model: %s\n", modelFilepath);

  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;

  bool ret =
      tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelFilepath);

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
        if (idx.normal_index >= 0) {
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
        if (idx.texcoord_index >= 0) {
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
      // shapes[s].mesh.material_ids[f];
    }
  }

  // Load texture
  texture.loadTexture2D(textureFilepath);
}

void GLObject::_bufferData() {
  // VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // VBO
  vbo.createVBO();
  vbo.bindVBO();
  vbo.addRawData(vertices.data(), vertices.size() * sizeof(Vertex));
  vbo.uploadDataToGPU(GL_STATIC_DRAW);

  // Shader input attrib
  const GLuint VERTEX_ATTR_POSITION = 0;
  const GLuint VERTEX_ATTR_NORMAL = 1;
  const GLuint VERTEX_ATTR_UV = 2;
  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
  glEnableVertexAttribArray(VERTEX_ATTR_UV);
  glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex),
                        (const GLvoid*)offsetof(Vertex, position));
  glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex),
                        (const GLvoid*)offsetof(Vertex, normal));
  glVertexAttribPointer(VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (const GLvoid*)offsetof(Vertex, uv));

  vbo.unbindVBO();
  glBindVertexArray(0);
}

void GLObject::draw(Uniform textureSampler) {
  // Bind our texture in Texture Unit 0
  texture.bind();

  // Set our texture sampler to use Texture Unit 0
  textureSampler = 0;

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glBindVertexArray(0);
}
