#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

#include "vertex.hpp"

namespace BufferData {
// Cube vertices positions
static constexpr GLfloat vertex_buffer_data[] = {
    -1.0f, -1.0f, -1.0f, //
    -1.0f, -1.0f, 1.0f,  //
    -1.0f, 1.0f,  1.0f,  //
    1.0f,  1.0f,  -1.0f, //
    -1.0f, -1.0f, -1.0f, //
    -1.0f, 1.0f,  -1.0f, //
    1.0f,  -1.0f, 1.0f,  //
    -1.0f, -1.0f, -1.0f, //
    1.0f,  -1.0f, -1.0f, //
    1.0f,  1.0f,  -1.0f, //
    1.0f,  -1.0f, -1.0f, //
    -1.0f, -1.0f, -1.0f, //
    -1.0f, -1.0f, -1.0f, //
    -1.0f, 1.0f,  1.0f,  //
    -1.0f, 1.0f,  -1.0f, //
    1.0f,  -1.0f, 1.0f,  //
    -1.0f, -1.0f, 1.0f,  //
    -1.0f, -1.0f, -1.0f, //
    -1.0f, 1.0f,  1.0f,  //
    -1.0f, -1.0f, 1.0f,  //
    1.0f,  -1.0f, 1.0f,  //
    1.0f,  1.0f,  1.0f,  //
    1.0f,  -1.0f, -1.0f, //
    1.0f,  1.0f,  -1.0f, //
    1.0f,  -1.0f, -1.0f, //
    1.0f,  1.0f,  1.0f,  //
    1.0f,  -1.0f, 1.0f,  //
    1.0f,  1.0f,  1.0f,  //
    1.0f,  1.0f,  -1.0f, //
    -1.0f, 1.0f,  -1.0f, //
    1.0f,  1.0f,  1.0f,  //
    -1.0f, 1.0f,  -1.0f, //
    -1.0f, 1.0f,  1.0f,  //
    1.0f,  1.0f,  1.0f,  //
    -1.0f, 1.0f,  1.0f,  //
    1.0f,  -1.0f, 1.0f,  //
};

// Cube vertices colors
static constexpr GLfloat color_buffer_data[] = {
    0.583f, 0.771f, 0.014f, //
    0.609f, 0.115f, 0.436f, //
    0.327f, 0.483f, 0.844f, //
    0.822f, 0.569f, 0.201f, //
    0.435f, 0.602f, 0.223f, //
    0.310f, 0.747f, 0.185f, //
    0.597f, 0.770f, 0.761f, //
    0.559f, 0.436f, 0.730f, //
    0.359f, 0.583f, 0.152f, //
    0.483f, 0.596f, 0.789f, //
    0.559f, 0.861f, 0.639f, //
    0.195f, 0.548f, 0.859f, //
    0.014f, 0.184f, 0.576f, //
    0.771f, 0.328f, 0.970f, //
    0.406f, 0.615f, 0.116f, //
    0.676f, 0.977f, 0.133f, //
    0.971f, 0.572f, 0.833f, //
    0.140f, 0.616f, 0.489f, //
    0.997f, 0.513f, 0.064f, //
    0.945f, 0.719f, 0.592f, //
    0.543f, 0.021f, 0.978f, //
    0.279f, 0.317f, 0.505f, //
    0.167f, 0.620f, 0.077f, //
    0.347f, 0.857f, 0.137f, //
    0.055f, 0.953f, 0.042f, //
    0.714f, 0.505f, 0.345f, //
    0.783f, 0.290f, 0.734f, //
    0.722f, 0.645f, 0.174f, //
    0.302f, 0.455f, 0.848f, //
    0.225f, 0.587f, 0.040f, //
    0.517f, 0.713f, 0.338f, //
    0.053f, 0.959f, 0.120f, //
    0.393f, 0.621f, 0.362f, //
    0.673f, 0.211f, 0.457f, //
    0.820f, 0.883f, 0.371f, //
    0.982f, 0.099f, 0.879f, //
};

static constexpr GLfloat uv_buffer_data[] = {
    0.000059f, 1.0f - 0.000004f, //
    0.000103f, 1.0f - 0.336048f, //
    0.335973f, 1.0f - 0.335903f, //
    1.000023f, 1.0f - 0.000013f, //
    0.667979f, 1.0f - 0.335851f, //
    0.999958f, 1.0f - 0.336064f, //
    0.667979f, 1.0f - 0.335851f, //
    0.336024f, 1.0f - 0.671877f, //
    0.667969f, 1.0f - 0.671889f, //
    1.000023f, 1.0f - 0.000013f, //
    0.668104f, 1.0f - 0.000013f, //
    0.667979f, 1.0f - 0.335851f, //
    0.000059f, 1.0f - 0.000004f, //
    0.335973f, 1.0f - 0.335903f, //
    0.336098f, 1.0f - 0.000071f, //
    0.667979f, 1.0f - 0.335851f, //
    0.335973f, 1.0f - 0.335903f, //
    0.336024f, 1.0f - 0.671877f, //
    1.000004f, 1.0f - 0.671847f, //
    0.999958f, 1.0f - 0.336064f, //
    0.667979f, 1.0f - 0.335851f, //
    0.668104f, 1.0f - 0.000013f, //
    0.335973f, 1.0f - 0.335903f, //
    0.667979f, 1.0f - 0.335851f, //
    0.335973f, 1.0f - 0.335903f, //
    0.668104f, 1.0f - 0.000013f, //
    0.336098f, 1.0f - 0.000071f, //
    0.000103f, 1.0f - 0.336048f, //
    0.000004f, 1.0f - 0.671870f, //
    0.336024f, 1.0f - 0.671877f, //
    0.000103f, 1.0f - 0.336048f, //
    0.336024f, 1.0f - 0.671877f, //
    0.335973f, 1.0f - 0.335903f, //
    0.667969f, 1.0f - 0.671889f, //
    1.000004f, 1.0f - 0.671847f, //
    0.667979f, 1.0f - 0.335851f, //
};

static Vertex3dColor colored_cube_data[] = {
    Vertex3dColor(glm::vec3(-1.0f, -1.0f, -1.0f),
                  glm::vec3(0.583f, 0.771f, 0.014f)),
    Vertex3dColor(glm::vec3(-1.0f, -1.0f, 1.0f),
                  glm::vec3(0.609f, 0.115f, 0.436f)),
    Vertex3dColor(glm::vec3(-1.0f, 1.0f, 1.0f),
                  glm::vec3(0.327f, 0.483f, 0.844f)),
    Vertex3dColor(glm::vec3(1.0f, 1.0f, -1.0f),
                  glm::vec3(0.822f, 0.569f, 0.201f)),
    Vertex3dColor(glm::vec3(-1.0f, -1.0f, -1.0f),
                  glm::vec3(0.435f, 0.602f, 0.223f)),
    Vertex3dColor(glm::vec3(-1.0f, 1.0f, -1.0f),
                  glm::vec3(0.31f, 0.747f, 0.185f)),
    Vertex3dColor(glm::vec3(1.0f, -1.0f, 1.0f),
                  glm::vec3(0.597f, 0.77f, 0.761f)),
    Vertex3dColor(glm::vec3(-1.0f, -1.0f, -1.0f),
                  glm::vec3(0.559f, 0.436f, 0.73f)),
    Vertex3dColor(glm::vec3(1.0f, -1.0f, -1.0f),
                  glm::vec3(0.359f, 0.583f, 0.152f)),
    Vertex3dColor(glm::vec3(1.0f, 1.0f, -1.0f),
                  glm::vec3(0.483f, 0.596f, 0.789f)),
    Vertex3dColor(glm::vec3(1.0f, -1.0f, -1.0f),
                  glm::vec3(0.559f, 0.861f, 0.639f)),
    Vertex3dColor(glm::vec3(-1.0f, -1.0f, -1.0f),
                  glm::vec3(0.195f, 0.548f, 0.859f)),
    Vertex3dColor(glm::vec3(-1.0f, -1.0f, -1.0f),
                  glm::vec3(0.014f, 0.184f, 0.576f)),
    Vertex3dColor(glm::vec3(-1.0f, 1.0f, 1.0f),
                  glm::vec3(0.771f, 0.328f, 0.97f)),
    Vertex3dColor(glm::vec3(-1.0f, 1.0f, -1.0f),
                  glm::vec3(0.406f, 0.615f, 0.116f)),
    Vertex3dColor(glm::vec3(1.0f, -1.0f, 1.0f),
                  glm::vec3(0.676f, 0.977f, 0.133f)),
    Vertex3dColor(glm::vec3(-1.0f, -1.0f, 1.0f),
                  glm::vec3(0.971f, 0.572f, 0.833f)),
    Vertex3dColor(glm::vec3(-1.0f, -1.0f, -1.0f),
                  glm::vec3(0.14f, 0.616f, 0.489f)),
    Vertex3dColor(glm::vec3(-1.0f, 1.0f, 1.0f),
                  glm::vec3(0.997f, 0.513f, 0.064f)),
    Vertex3dColor(glm::vec3(-1.0f, -1.0f, 1.0f),
                  glm::vec3(0.945f, 0.719f, 0.592f)),
    Vertex3dColor(glm::vec3(1.0f, -1.0f, 1.0f),
                  glm::vec3(0.543f, 0.021f, 0.978f)),
    Vertex3dColor(glm::vec3(1.0f, 1.0f, 1.0f),
                  glm::vec3(0.279f, 0.317f, 0.505f)),
    Vertex3dColor(glm::vec3(1.0f, -1.0f, -1.0f),
                  glm::vec3(0.167f, 0.62f, 0.077f)),
    Vertex3dColor(glm::vec3(1.0f, 1.0f, -1.0f),
                  glm::vec3(0.347f, 0.857f, 0.137f)),
    Vertex3dColor(glm::vec3(1.0f, -1.0f, -1.0f),
                  glm::vec3(0.055f, 0.953f, 0.042f)),
    Vertex3dColor(glm::vec3(1.0f, 1.0f, 1.0f),
                  glm::vec3(0.714f, 0.505f, 0.345f)),
    Vertex3dColor(glm::vec3(1.0f, -1.0f, 1.0f),
                  glm::vec3(0.783f, 0.29f, 0.734f)),
    Vertex3dColor(glm::vec3(1.0f, 1.0f, 1.0f),
                  glm::vec3(0.722f, 0.645f, 0.174f)),
    Vertex3dColor(glm::vec3(1.0f, 1.0f, -1.0f),
                  glm::vec3(0.302f, 0.455f, 0.848f)),
    Vertex3dColor(glm::vec3(-1.0f, 1.0f, -1.0f),
                  glm::vec3(0.225f, 0.587f, 0.04f)),
    Vertex3dColor(glm::vec3(1.0f, 1.0f, 1.0f),
                  glm::vec3(0.517f, 0.713f, 0.338f)),
    Vertex3dColor(glm::vec3(-1.0f, 1.0f, -1.0f),
                  glm::vec3(0.053f, 0.959f, 0.12f)),
    Vertex3dColor(glm::vec3(-1.0f, 1.0f, 1.0f),
                  glm::vec3(0.393f, 0.621f, 0.362f)),
    Vertex3dColor(glm::vec3(1.0f, 1.0f, 1.0f),
                  glm::vec3(0.673f, 0.211f, 0.457f)),
    Vertex3dColor(glm::vec3(-1.0f, 1.0f, 1.0f),
                  glm::vec3(0.82f, 0.883f, 0.371f)),
    Vertex3dColor(glm::vec3(1.0f, -1.0f, 1.0f),
                  glm::vec3(0.982f, 0.099f, 0.879f)),
};

static Vertex3dUV textured_cube_data[] = {
    Vertex3dUV(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
    Vertex3dUV(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.666666f)),
    Vertex3dUV(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.333333f, 0.666666f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex3dUV(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.666666f, 0.666666f)),
    Vertex3dUV(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.666666f)),
    Vertex3dUV(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.666666f, 0.666666f)),
    Vertex3dUV(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.333333f, 0.333333f)),
    Vertex3dUV(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.666666f, 0.333333f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
    Vertex3dUV(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.666666f, 1.0f)),
    Vertex3dUV(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.666666f, 0.666666f)),
    Vertex3dUV(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
    Vertex3dUV(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.333333f, 0.666666f)),
    Vertex3dUV(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.333333f, 1.0f)),
    Vertex3dUV(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.666666f, 0.666666f)),
    Vertex3dUV(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.333333f, 0.666666f)),
    Vertex3dUV(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.333333f, 0.333333f)),
    Vertex3dUV(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.333333f)),
    Vertex3dUV(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.999958f, 0.666666f)),
    Vertex3dUV(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.666666f, 0.666666f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.666666f, 1.0f)),
    Vertex3dUV(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.333333f, 0.666666f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.666666f, 0.666666f)),
    Vertex3dUV(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.333333f, 0.666666f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.666666f, 1.0f)),
    Vertex3dUV(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.666666f, 1.0f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.666666f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.333333)),
    Vertex3dUV(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.333333f, 0.333333f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.666666f)),
    Vertex3dUV(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.333333f, 0.333333f)),
    Vertex3dUV(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.333333f, 0.666666f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.666666f, 0.333333f)),
    Vertex3dUV(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.333333f)),
    Vertex3dUV(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.666666f, 0.666666f)),
};

static Vertex3dUV textured_cube_indexed_data[] = {
    Vertex3dUV(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.66f)),
    Vertex3dUV(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.25f, 0.66f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.33f)),
    Vertex3dUV(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.25f, 0.33f)),

    Vertex3dUV(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 0.66f)),
    Vertex3dUV(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.5f, 0.33f)),
    Vertex3dUV(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(0.75f, 0.66f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.75f, 0.33f)),

    Vertex3dUV(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.66f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.33f)),

    Vertex3dUV(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.25f, 1.0f)),
    Vertex3dUV(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(0.5f, 1.0f)),

    Vertex3dUV(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.25f, 0.0f)),
    Vertex3dUV(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.0f)),

};

static uint32_t textured_cube_indexed_indices[] = {
    0, 2,  1,  // front
    1, 2,  3,  //
    4, 5,  6,  // back
    5, 7,  6,  //
    6, 7,  8,  // top
    7, 9,  8,  //
    1, 3,  4,  // bottom
    3, 5,  4,  //
    1, 11, 10, // left
    1, 4,  11, //
    3, 12, 5,  // right
    5, 12, 13  //
};

}; // namespace BufferData