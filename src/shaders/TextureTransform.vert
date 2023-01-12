#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 vFragUV;
out vec3 vVertexPosition;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main() {
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(aVertexPosition, 1);
	
	// UV of the vertex. No special space for this one.
	vFragUV = aVertexUV;

	vVertexPosition = aVertexPosition;
}

