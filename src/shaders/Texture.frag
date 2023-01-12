#version 330 core

// Interpolated values from the vertex shaders
in vec2 vFragUV;
in vec3 vVertexPosition;

// Ouput data
out vec3 fFragColor;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;

void main() {

// Attenuation based on distance to center
	float alpha = 1.5;
	float beta = 0.5;
	float distanceToCenter = length(vVertexPosition);
	float attenuation = alpha * exp(-beta * distanceToCenter * distanceToCenter);
	attenuation = min(1, attenuation);

	// Output color = color of the texture at the specified UV
	// fFragColor = texture(textureSampler, vFragUV).rgb * attenuation;
	fFragColor = vec3(1);
}