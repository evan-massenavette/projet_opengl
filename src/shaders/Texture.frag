#version 330 core

#define MAX_NUM_DIRECTIONAL_LIGHTS 8
struct DirectionalLight {
	vec3 direction;
	vec3 color;
	float factor;
	bool isOn;
};

vec3 getDirectionalLightColor(DirectionalLight directionalLight, vec3 normal)
{
    if(!directionalLight.isOn) return vec3(0);
    
    float finalIntensity = max(0.0, dot(normal, -directionalLight.direction));
    finalIntensity = clamp(finalIntensity*directionalLight.factor, 0.0, 1.0);
    return vec3(directionalLight.color*finalIntensity);
}



// Inputs
in vec3 vVertexNormal;
in vec2 vVertexUV;
in vec3 vVertexWorldPosition;
in vec3 vVertexCameraSpacePosition;

// Outputs
out vec3 fFinalColor;

// Texturing uniforms
uniform sampler2D textureSampler;

// Lighting uniforms
layout (std140) uniform DirectionalLightsBlock {
	int count;
	DirectionalLight data[MAX_NUM_DIRECTIONAL_LIGHTS];
} directionalLights;

float max3(vec3 v) {
	return max(v.x, max(v.y, v.z));
}
float min3(vec3 v) {
	return min(v.x, min(v.y, v.z));
}

void main() {
	// Normal and texture color
	vec3 normal = normalize(vVertexNormal);
    vec3 textureColor = texture(textureSampler, vVertexUV).rgb;

	// Ambient lights
    // vec3 ambientColor = getAmbientLightColor(ambientLight);

	// Directional lights
	vec3 diffuseColor = vec3(0,0,0);
	for (int i = 0; i < directionalLights.count; i++) {
		diffuseColor += getDirectionalLightColor(directionalLights.data[i], normal);
	}

	// Specular lights
	// vec3 specularHighlightColor = getSpecularHighlightColor(ioWorldPosition.xyz, normal, eyePosition, material, directionalLight);

	// vec3 lightColor = ambientColor + diffuseColor + specularHighlightColor;
	vec3 lightColor = diffuseColor;

	// Point lights
	// for(int i = 0; i < numPointLights; i++) {
	// 	lightColor += getPointLightColor(block_pointLights.lights[i], ioWorldPosition.xyz, normal);
	// }
    
    fFinalColor = textureColor * lightColor;
}