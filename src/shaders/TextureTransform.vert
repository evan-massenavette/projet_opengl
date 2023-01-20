#version 330 core

// Inputs
layout(location = 0) in vec3 aVertexPos;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexUV;

// Outputs
out vec3 vFragNormal;
out vec2 vFragUV;
out vec3 vFragWorldPos;
out vec3 vFragCameraSpacePos;

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
	gl_Position = mvpMatrix * vec4(aVertexPos, 1.0);

	// Output all out variables
	vFragNormal = matrices.normal * aVertexNormal;
	vFragUV = aVertexUV;
	vFragWorldPos = (matrices.model * vec4(aVertexPos, 1.0)).xyz;
	vFragCameraSpacePos = (mvMatrix * vec4(aVertexPos, 1.0)).xyz;
}
