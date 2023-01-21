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
	vec3 direction;
	vec3 color;
	float intensityFactor;
	bool isOn;
};

const int MAX_POINT_LIGHTS = 8;
struct PointLight {
	vec3 position;
	vec3 color;
	float intensityFactor;
	bool isOn;
};

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
	float diffuseIntensity = max(0.0, dot(normal, -directionalLight.direction));
	vec3 diffuseColor = directionalLight.color * diffuseIntensity * material.diffuse;

	// Specular lighting
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(directionalLight.direction, normal);
	float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = directionalLight.color * specularIntensity * material.specular;

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
	float diffuseIntensity = clamp(dot(normal, -lightToFragDir), 0.0, 1.0);
	vec3 diffuseColor = pointLight.color * diffuseIntensity * material.diffuse;

	// Specular lighting
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(lightToFragDir, normal);
	float specularIntensity = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = pointLight.color * specularIntensity * material.specular;

	vec3 finalColor = (diffuseColor + specularColor) * pointLight.intensityFactor;

	// Apply attenuation based on distance
	float attenuation = 1.0 / (1.0 + lightToFragDistance * lightToFragDistance);
	finalColor *= attenuation;

	return clamp(finalColor, 0.0, 1.0);
}

// Inputs
in vec3 vFragNormal;
in vec2 vFragUV;
in vec3 vFragWorldPos;
in vec3 vFragCameraSpacePos;

// Outputs
out vec3 fFinalColor;

// Texturing uniforms
uniform sampler2D textureSampler;

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
	vec3 normal = normalize(vFragNormal);
	vec3 textureColor = texture(textureSampler, vFragUV).rgb;

	// Ambient lights
	for(int i = 0; i < ambientLights.count; i++) {
		AmbientLight ambientLight = ambientLights.data[i];
		fFinalColor += getAmbientLightColor(ambientLight, material);
	}

	// Directional lights
	for(int i = 0; i < directionalLights.count; i++) {
		DirectionalLight directionalLight = directionalLights.data[i];
		fFinalColor += getDirectionalLightColor(directionalLight, material, normal, cameraWorldPos, vFragWorldPos);
	}

	// Point lights
	for(int i = 0; i < pointLights.count; i++) {
		PointLight pointLight = pointLights.data[i];
		fFinalColor += getPointLightColor(pointLight, material, normal, cameraWorldPos, vFragWorldPos);
	}

	fFinalColor *= textureColor;
}