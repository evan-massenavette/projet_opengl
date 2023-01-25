#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

// Matrices uniforms
uniform struct {
    mat4 projection;
    mat4 view;
    mat4 model;
} matrices;

out vec3 gWorldPos;

void main() {
    for(int face = 0; face < 6; face++) { // For each face of the depth cube map
        gl_Layer = face; // Which face we render
        for(int i = 0; i < 3; ++i) // For each vertex of the triangle
        {
            vec4 fragPos = gl_in[i].gl_Position;
            gl_Position = matrices.projection * matrices.view * fragPos;
            gWorldPos = fragPos.xyz;
            EmitVertex();
        }
        EndPrimitive();
    }
}