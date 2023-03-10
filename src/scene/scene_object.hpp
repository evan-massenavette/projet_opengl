#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "../gl_wrappers/texture.hpp"
#include "../gl_wrappers/vertex_buffer_object.hpp"
#include "../render_pass.hpp"
#include "scene_object_material.hpp"
#include "vertex.hpp"

/**
 * Class representing an object in a scene.
 */
class SceneObject {
 public:
  /**
   * Construct a new SceneObject.
   * @param modelName Name of the model to load
   */
  SceneObject(const std::string& modelName,
              const glm::vec3& position = glm::vec3(0),
              const glm::vec3& rotation = glm::vec3(0),
              const glm::vec3& scale = glm::vec3(1));

  /**
   * Disabled copy constructor.
   */
  SceneObject(const SceneObject&) = delete;

  /**
   * Disabled copy assignment operator.
   */
  SceneObject& operator=(const SceneObject&) = delete;

  /**
   * Destroy the SceneObject
   */
  ~SceneObject();

  /**
   * Draw the object.
   */
  void draw(RenderPass renderPass);

  /**
   * Load the given model
   * @param modelName Name of the model to load
   */
  void _loadModel(const std::string& modelName);

  /**
   * Buffer all the data loaded in this object
   */
  void _bufferData();

  /**
   * Set the object's scale (in model coordinates)
   * @param factors The x,y,z scale factors to set for the object
   */
  void setScale(const glm::vec3& factors);

  /**
   * Add a rotation to the object (in model coordinates)
   * @param angles The x,y,z angles to add to the object's current rotation
   */
  void rotate(const glm::vec3& angles);

  /**
   * Set the object's rotation (in model coordinates)
   * @param angles The x,y,z angles to set the object's rotation to
   */
  void setRotation(const glm::vec3& angles);

  /**
   * Translate the object's position (in model coordinates)
   * @param distances The x,y,z angles to add to the object's position
   */
  void translate(const glm::vec3& distances);

  /**
   * Set the object's position (in world coordinates)
   * @param position The x,y,z coordinates to put the model at in the world
   */
  void setPosition(const glm::vec3& distances);

  const glm::vec3 getPosition() const;
  const glm::vec3 getRotation() const;
  const glm::vec3 getScale() const;

 private:
  std::vector<std::unique_ptr<SceneObjectMaterial>> _objectMaterials;

  glm::vec3 _position;
  glm::vec3 _rotation;
  glm::vec3 _scale;

  // Flag for if the object has been changed since last draw.
  // True at the beginning so that the object gets initialized.
  bool _hasChanged = true;

  glm::mat4 _modelMatrix;  // Cached model matrix
  /**
   * Computes the model matrix of this object
   * based on its position, rotation, and scale.
   * @return The model matrix of this object
   */
  glm::mat4 _getModelMatrix();
};

#endif