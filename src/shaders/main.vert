#version 330 core

// Inputs
layout(location = 0) in vec3 aModelPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

// Outputs
out vec3 vNormal;
out vec2 vUV;
out vec3 vWorldPos;
out vec3 vCameraSpacePos;

// Matrices uniforms
uniform struct {
	mat4 projection;
	mat4 view;
	mat4 model;
	mat3 normal;
} matrices;

void main() {
	// Compute matrices
	mat4 mvMatrix = matrices.view * matrices.model;
	mat4 mvpMatrix = matrices.projection * mvMatrix;

	// Clip space position
	gl_Position = mvpMatrix * vec4(aModelPos, 1.0);

	// Output all out variables
	vNormal = matrices.normal * aNormal;
	vUV = aUV;
	vWorldPos = (matrices.model * vec4(aModelPos, 1.0)).xyz;
	vCameraSpacePos = (mvMatrix * vec4(aModelPos, 1.0)).xyz;
}
