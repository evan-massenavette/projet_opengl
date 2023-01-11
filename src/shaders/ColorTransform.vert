#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aVertexPositionModelSpace;
layout(location = 1) in vec3 aVertexColor;

// Output data ; will be interpolated for each fragment.
out vec3 vFragColor;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main() {
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(aVertexPositionModelSpace, 1);

	// The color of each vertex will be interpolated to produce the color of each fragment
    vFragColor = aVertexColor;
}

