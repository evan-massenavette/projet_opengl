#version 330 core

// Interpolated values from the vertex shaders
in vec3 vFragColor;

// Output data
out vec3 fFragColor;

void main() {
	// Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices
	fFragColor = vFragColor;
}
