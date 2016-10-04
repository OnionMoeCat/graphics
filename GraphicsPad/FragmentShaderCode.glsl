#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;
uniform vec4 ambientLight;

uniform float k0;
uniform float k1;
uniform float k2;

void main()
{
	vec3 lightVectorWorld = lightPositionWorld - vertexPositionWorld;
	float distance = length(lightVectorWorld);
	float diffuseBrightness = clamp(dot(normalize(lightVectorWorld), normalize(normalWorld)) / (k0 + k1 * distance + k2 * distance * distance), 0, 1);
	vec4 diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1.0);

	vec3 eyeVectorWorld = eyePositionWorld - vertexPositionWorld;
	vec3 lightVectorReflect = reflect(-eyeVectorWorld, normalWorld);
	float specularBrightness = clamp(dot(normalize(lightVectorReflect), normalize(eyeVectorWorld)), 0, 1);
	pow(specularBrightness, 20);
	vec4 specularLight = vec4(specularBrightness, 0.0, 0.0, 1.0);

	daColor = diffuseLight + ambientLight + specularLight;
}