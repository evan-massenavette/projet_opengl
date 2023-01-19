#version 330 core

// Inputs
layout (location = 0) in vec3 aVertexPosition;
layout (location = 1) in vec3 aVertexNormal;
layout (location = 2) in vec2 aVertexUV;

// Outputs
out vec3 vVertexNormal;
out vec2 vVertexUV;
out vec3 vVertexWorldPosition;
out vec3 vVertexCameraSpacePosition;

// Matrices uniforms
uniform struct
{
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
	gl_Position = mvpMatrix * vec4(aVertexPosition, 1.0);

	// Output all out variables
	vVertexNormal = matrices.normal * aVertexNormal;
	vVertexUV = aVertexUV;
	vVertexWorldPosition = (matrices.model * vec4(aVertexPosition, 1.0)).xyz;
	vVertexCameraSpacePosition = (mvMatrix * vec4(aVertexPosition, 1.0)).xyz;
}

