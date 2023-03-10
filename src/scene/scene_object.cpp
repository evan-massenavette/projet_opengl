#include <iostream>
#include <memory>
#include <string>
#include <utility>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <glm/ext/matrix_transform.hpp>

#include "../gl_wrappers/shader_program_manager.hpp"

#include "vertex.hpp"

#include "scene_object.hpp"

SceneObject::SceneObject(const std::string& modelName,
                         const glm::vec3& position,
                         const glm::vec3& rotation,
                         const glm::vec3& scale)
    : _position(position), _rotation(rotation), _scale(scale) {
  _loadModel(modelName);
  _bufferData();
  _getModelMatrix();  // Calculate model matrix for first time
  _hasChanged = false;
}

SceneObject::~SceneObject() {
  // Cleanup
  _objectMaterials.clear();
}

void SceneObject::_loadModel(const std::string& modelName) {
  std::cout << "Loading model: " << modelName << "\n";

  // Vars that will contain loaded model
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;

  // Load the model
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
                              ("models/" + modelName + "/model.obj").c_str(),
                              ("models/" + modelName + "/").c_str());

  if (!err.empty()) {
    std::cerr << err << "\n";
  }

  if (!ret) {
    exit(EXIT_FAILURE);
  }
  std::cout << "(";
  std::cout << attrib.vertices.size() << " vertices, ";
  std::cout << attrib.normals.size() << " normals, ";
  std::cout << attrib.texcoords.size() << " texcoords";
  std::cout << ")\n";

  // Load object materials
  for (tinyobj::material_t& material : materials) {
    // Load material elements
    glm::vec3 ambient(material.ambient[0], material.ambient[1],
                      material.ambient[2]);
    glm::vec3 diffuse(material.diffuse[0], material.diffuse[1],
                      material.diffuse[2]);
    glm::vec3 specular(material.specular[0], material.specular[1],
                       material.specular[2]);
    float shininess = material.shininess;
    shader_structs::Material modelMaterial(ambient, diffuse, specular,
                                           shininess);
    auto objectMaterial = new SceneObjectMaterial(modelMaterial);

    // Load texture
    std::string textureFilename = material.diffuse_texname;
    if (textureFilename.empty())  // When no texture given
    {
      objectMaterial->texture = nullptr;
    } else {
      std::shared_ptr<Texture> texture = std::make_shared<Texture>();
      bool isTextureLoaded = texture->loadTexture2D(
          "models/" + modelName + "/textures/" + textureFilename);
      if (isTextureLoaded)  // When existent texture given
      {
        objectMaterial->texture = texture;
      } else  // When inexistent texture given
      {
        objectMaterial->texture = Texture::getMissingTexture();
      }
    }

    _objectMaterials.emplace_back(objectMaterial);
  }

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    // Loop over faces(polygon)
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      // per-face material
      int idx_material = shapes[s].mesh.material_ids[f];

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
        _objectMaterials[idx_material]->vertices.push_back(vertex);
      }
      index_offset += fv;
    }
  }
}

void SceneObject::draw(RenderPass renderPass) {
  if (renderPass == RenderPass::Depth) {
    auto& depthProgram = ShaderProgramManager::getInstance().getShaderProgram(
        ShaderProgramKeys::depth());

    // Set the model and normal matrix for this object
    depthProgram[ShaderConstants::modelMatrix()] = _getModelMatrix();
  }

  else if (renderPass == RenderPass::Main) {
    auto& mainProgram = ShaderProgramManager::getInstance().getShaderProgram(
        ShaderProgramKeys::main());

    // Set the model and normal matrix for this object
    mainProgram.setModelAndNormalMatrix(_getModelMatrix());
  }

  // Draw all materials
  for (auto& objectMaterial : _objectMaterials) {
    objectMaterial->draw(renderPass);
  }

  // Set hasChanged flag
  _hasChanged = false;
}

void SceneObject::_bufferData() {
  for (auto& objectMaterial : _objectMaterials) {
    objectMaterial->bufferData();
  }
}

void SceneObject::setScale(const glm::vec3& factors) {
  _scale = factors;
  _hasChanged = true;
}

void SceneObject::rotate(const glm::vec3& angles) {
  _rotation += angles;
  _hasChanged = true;
}
void SceneObject::setRotation(const glm::vec3& angles) {
  _rotation = angles;
  _hasChanged = true;
}
void SceneObject::translate(const glm::vec3& distances) {
  _position += distances;
  _hasChanged = true;
}
void SceneObject::setPosition(const glm::vec3& distances) {
  _position = distances;
  _hasChanged = true;
}

const glm::vec3 SceneObject::getPosition() const {
  return _position;
}

const glm::vec3 SceneObject::getRotation() const {
  return _rotation;
}

const glm::vec3 SceneObject::getScale() const {
  return _scale;
}

glm::mat4 SceneObject::_getModelMatrix() {
  // If the object hasn't changed, return cached model matrix
  if (!_hasChanged) {
    return _modelMatrix;
  }

  // Recalculate the model matrix

  // Base model matrix = Identity Matrix
  glm::mat4 baseModelMatrix(1);

  // Scale
  auto scale = glm::scale(glm::mat4(1), _scale);

  // Rotate
  glm::vec3 x_axis(1, 0, 0);
  glm::vec3 y_axis(0, 1, 0);
  glm::vec3 z_axis(0, 0, 1);
  auto rotate_x = glm::rotate(glm::mat4(1), _rotation.x, x_axis);
  auto rotate_xy = glm::rotate(rotate_x, _rotation.y, y_axis);
  auto rotate_xyz = glm::rotate(rotate_xy, _rotation.z, z_axis);

  // Translate
  auto translate = glm::translate(glm::mat4(1), _position);

  // Cache the new model matrix
  _modelMatrix = translate * rotate_xyz * scale;

  return _modelMatrix;
}
