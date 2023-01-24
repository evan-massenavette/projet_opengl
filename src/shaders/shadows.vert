#version 330 core

// Inputs
layout(location = 0) in vec3 aModelPos;

// Matrices uniforms
uniform struct {
    mat4 projection;
    mat4 view;
    mat4 model;
} matrices;

void main() {
	// Compute matrices
    mat4 mvpMatrix = matrices.projection * matrices.view * matrices.model;

	// Clip space position
    gl_Position = mvpMatrix * vec4(aModelPos, 1.0);
}
