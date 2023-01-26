#include "scene_object_material.hpp"
#include "../gl_wrappers/shader_program_manager.hpp"

SceneObjectMaterial::SceneObjectMaterial(shader_structs::Material material)
    : material{material} {}

SceneObjectMaterial::~SceneObjectMaterial() {
  vbo.deleteVBO();
  glDeleteVertexArrays(1, &vao);
}

void SceneObjectMaterial::bufferData() {
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

void SceneObjectMaterial::draw(RenderPass renderPass) {
  if (renderPass == RenderPass::Depth) {
  }

  if (renderPass == RenderPass::Main) {
    // Send Material to shader
    auto& mainProgram = ShaderProgramManager::getInstance().getShaderProgram(
        ShaderProgramKeys::main());
    material.setUniform(mainProgram, ShaderConstants::material());

    bool hasTexture = texture != nullptr;
    mainProgram[ShaderConstants::missingTexture()] = !hasTexture;

    if (hasTexture) {
      // Bind our texture
      texture->bind();
      // Set our albedo sampler to use Texture Unit 0
      mainProgram[ShaderConstants::albedoSampler()] = 0;

      // Bind our texture to texture unit
      GLint albedoTexUnit = 0;
      texture->bind(albedoTexUnit);
      // Set our albedo sampler to use Texture Unit
      mainProgram[ShaderConstants::albedoSampler()] = albedoTexUnit;
    }
  }

  // Draw
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
  glBindVertexArray(0);
}