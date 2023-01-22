#include "face.hpp"

Face::Face(std::vector<Vertex> vertices, GLuint texture, shader_structs::Material material) : vertices{vertices}, texture{texture}, material{material} {}

Face::~Face()
{
    vbo.deleteVBO();
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &texture);
}

void Face::bufferData()
{
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
                          (const GLvoid *)offsetof(Vertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),
                          (const GLvoid *)offsetof(Vertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (const GLvoid *)offsetof(Vertex, uv));

    vbo.unbindVBO();
    glBindVertexArray(0);
}
void Face::draw(Uniform textureSampler)
{
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our texture sampler to use Texture Unit 0
    textureSampler = 0;

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}