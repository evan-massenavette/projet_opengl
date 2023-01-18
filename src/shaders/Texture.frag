#version 330 core

#define MAX_NUM_DIRECTIONAL_LIGHTS 8
struct DirectionalLight {
	vec3 direction;
	vec3 color;
};

// Inputs (interpolated values from vertex shader)
in vec3 vPosition;
in vec3 vNormal;
in vec2 vUV;

// Outputs
out vec3 fColor;

// Texturing uniforms
uniform sampler2D textureSampler;

// Lighting uniforms
layout (std140) uniform Lighting {
	uint numDirectionalLights;
	DirectionalLight directionalLigths[MAX_NUM_DIRECTIONAL_LIGHTS];
};

void main() {

// Attenuation based on distance to center
	float alpha = 1.5;
	float beta = 0.5;
	float distanceToCenter = length(vPosition);
	float attenuation = alpha * exp(-beta * distanceToCenter * distanceToCenter);
	attenuation = min(1, attenuation);

	// Output color = color of the texture at the specified UV
	fColor = texture(textureSampler, vUV).rgb;// * attenuation;
}