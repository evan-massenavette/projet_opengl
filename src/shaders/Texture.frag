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
	DirectionalLight directionalLights[MAX_NUM_DIRECTIONAL_LIGHTS];
};

float max3(vec3 v) {
	return max(v.x, max(v.y, v.z));
}
float min3(vec3 v) {
	return min(v.x, min(v.y, v.z));
}

void main() {
	// Ambient
	vec3 ambient = texture(textureSampler, vUV).rgb;

	// Diffuse
	vec3 diffuse = vec3(0);
	for (uint i = 0u; i < numDirectionalLights; i++) {
		DirectionalLight light = directionalLights[i];
		diffuse += max(dot(vNormal, light.direction), 0.0) * light.color;
	}

	// fColor = ambient + diffuse;
	fColor = diffuse;
	if (directionalLights[0].color.r > 0) {
	// if (numDirectionalLights == 1u) {
		fColor = vec3(1,0,0);
	}
}