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
	// Transform vertex to world space
    gl_Position = matrices.model * vec4(aModelPos, 1.0);
}
