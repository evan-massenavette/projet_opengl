#version 330 core

#define VERTICES_COUNT 3
#define VERTICES_COUNT 3

layout(triangles) in;
layout(triangle_strip, max_vertices = VERTICES_COUNT) out;

in vec3 vNormal[VERTICES_COUNT];
in vec2 vUV[VERTICES_COUNT];
in vec3 vWorldPos[VERTICES_COUNT];
in vec3 vCameraSpacePos[VERTICES_COUNT];

out vec3 gNormal;
out vec2 gUV;
out vec3 gWorldPos;
out vec3 gCameraSpacePos;

void main() {

    // Calculate new normal for this triangle
    vec3 v1Pos = gl_in[0].gl_Position.xyz;
    vec3 v2Pos = gl_in[1].gl_Position.xyz;
    vec3 v3Pos = gl_in[2].gl_Position.xyz;
    vec3 edge1 = v2Pos - v1Pos;
    vec3 edge2 = v3Pos - v1Pos;
    vec3 newNormal = normalize(cross(edge1, edge2));

    for(int i = 0; i < VERTICES_COUNT; i++) {
        // Set position
        gl_Position = gl_in[i].gl_Position;

        bool isMissingNormal = (vNormal[i] == vec3(0));

        // Set outputs
        gNormal = isMissingNormal ? newNormal : vNormal[i];
        gUV = vUV[i];
        gWorldPos = vWorldPos[i];
        gCameraSpacePos = vCameraSpacePos[i];

        // End the vertex
        EmitVertex();
    }
    // End the primitive
    EndPrimitive();
}