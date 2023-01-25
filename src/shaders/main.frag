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

vec3 computeAmbientLighting(vec3 lightColor, Material material) {
	vec3 ambientColor = lightColor * material.ambient;
	return ambientColor;
}

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

	vec3 ambientColor = computeAmbientLighting(ambientLight.color, material);
	vec3 finalColor = ambientColor * ambientLight.intensityFactor;

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

// Depth maps for shadows
uniform samplerCube depthSampler;
uniform float farPlane;

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

float calculateShadow(vec3 fragPos) {
	vec3 lightPos = pointLights.data[0].position;

    // Vector between fragment position and light position and its length
	vec3 lightToFrag = fragPos - lightPos;
	float currentDepth = length(lightToFrag);

    // Sample from the depth map and transform its value (in [0;1]) back to a distance
	float closestDepth = texture(depthSampler, -lightToFrag).r;
	closestDepth *= farPlane;

    // Test for shadows
	float bias = 0.05;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	// TEMP
	if(closestDepth == 0)
		return 2.0;

	return shadow;
}

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
		float shadow = calculateShadow(gWorldPos);
		if(shadow == 2.0) {
			fColor += vec3(0, 0, 1);
			continue;
		}
		fColor += (1.0 - shadow) * getPointLightColor(pointLight, material, normal, cameraWorldPos, gWorldPos);
	}

	fColor *= albedoColor;
}