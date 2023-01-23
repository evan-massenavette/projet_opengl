#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

const int MAX_AMBIENT_LIGHTS = 8;
struct AmbientLight {
	vec3 color;
	float intensityFactor;
	bool isOn;
};

const int MAX_DIRECTIONAL_LIGHTS = 8;
struct DirectionalLight {
	vec3 color;
	float intensityFactor;
	vec3 direction;
	bool isOn;
};

const int MAX_POINT_LIGHTS = 8;
struct PointLight {
	vec3 color;
	float intensityFactor;
	vec3 position;
	float attenuationFactor;
	bool isOn;
};

vec3 computeDiffuseLighting(vec3 normal, vec3 lightToFragDir, vec3 lightColor, Material material) {
	float diffuseIntensity = max(0.0, dot(normal, -lightToFragDir));
	vec3 diffuseColor = lightColor * diffuseIntensity * material.diffuse;
	return diffuseColor;
}

vec3 computeSpecularLighting(vec3 normal, vec3 lightToFragDir, vec3 cameraPos, vec3 fragPos, vec3 lightColor, Material material) {
	// Specular lighting
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(lightToFragDir, normal);
	float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = lightColor * specularIntensity * material.specular;
	return specularColor;
}

vec3 getAmbientLightColor(AmbientLight ambientLight, Material material) {
	// If light is off, return black
	if(!ambientLight.isOn)
		return vec3(0);

	vec3 finalColor = (ambientLight.color * material.ambient) * ambientLight.intensityFactor;

	return clamp(finalColor, 0.0, 1.0);
}

vec3 getDirectionalLightColor(DirectionalLight directionalLight, Material material, vec3 normal, vec3 cameraPos, vec3 fragPos) {

	// If light is off, return black
	if(!directionalLight.isOn)
		return vec3(0);

	// Diffuse lighting
	vec3 diffuseColor = computeDiffuseLighting(normal, directionalLight.direction, directionalLight.color, material);

	// Specular lighting
	vec3 specularColor = computeSpecularLighting(normal, directionalLight.direction, cameraPos, fragPos, directionalLight.color, material);

	vec3 finalColor = (diffuseColor + specularColor) * directionalLight.intensityFactor;

	return clamp(finalColor, 0.0, 1.0);
}

vec3 getPointLightColor(PointLight pointLight, Material material, vec3 normal, vec3 cameraPos, vec3 fragPos) {

	// If light is off, return black
	if(!pointLight.isOn)
		return vec3(0);

	// Vars used in rest of calculations
	vec3 lightToFragDir = normalize(fragPos - pointLight.position);
	float lightToFragDistance = distance(fragPos, pointLight.position);

	// Diffuse lighting
	vec3 diffuseColor = computeDiffuseLighting(normal, lightToFragDir, pointLight.color, material);

	// Specular lighting
	vec3 specularColor = computeSpecularLighting(normal, lightToFragDir, cameraPos, fragPos, pointLight.color, material);

	vec3 finalColor = (diffuseColor + specularColor) * pointLight.intensityFactor;

	// Apply attenuation based on distance
	float attenuation = 1.0 / (1.0 + pointLight.attenuationFactor * lightToFragDistance * lightToFragDistance);
	finalColor *= attenuation;

	return clamp(finalColor, 0.0, 1.0);
}

// Inputs
in vec3 gNormal;
in vec2 gUV;
in vec3 gWorldPos;
in vec3 gCameraSpacePos;

// Outputs
out vec3 fColor;

// Texturing uniforms
uniform sampler2D albedoSampler;

// Other uniforms
uniform vec3 cameraWorldPos;
uniform Material material;

// Lighting uniforms
layout(std140) uniform AmbientLightsBlock {
	int count;
	AmbientLight data[MAX_AMBIENT_LIGHTS];
} ambientLights;

layout(std140) uniform DirectionalLightsBlock {
	int count;
	DirectionalLight data[MAX_DIRECTIONAL_LIGHTS];
} directionalLights;

layout(std140) uniform PointLightsBlock {
	int count;
	PointLight data[MAX_POINT_LIGHTS];
} pointLights;

void main() {
	// Normal and texture color
	vec3 normal = normalize(gNormal);
	vec3 albedoColor = texture(albedoSampler, gUV).rgb;

	// Ambient lights
	for(int i = 0; i < ambientLights.count; i++) {
		AmbientLight ambientLight = ambientLights.data[i];
		fColor += getAmbientLightColor(ambientLight, material);
	}

	// Directional lights
	for(int i = 0; i < directionalLights.count; i++) {
		DirectionalLight directionalLight = directionalLights.data[i];
		fColor += getDirectionalLightColor(directionalLight, material, normal, cameraWorldPos, gWorldPos);
	}

	// Point lights
	for(int i = 0; i < pointLights.count; i++) {
		PointLight pointLight = pointLights.data[i];
		fColor += getPointLightColor(pointLight, material, normal, cameraWorldPos, gWorldPos);
	}

	fColor *= albedoColor;
}