#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

// Output data (will be interpolated for each fragment)
out vec3 vPosition;
out vec3 vNormal;
out vec2 vUV;

// Matrices uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	// Output position of the vertex, in clip space
	gl_Position = projection * view * model * vec4(aPosition, 1.0);

	// UV Coordinates
	vUV = aUV;

	// Vertex coordinates in model space
	vPosition = aPosition;
}

