#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 lightPositionWorld;
uniform vec4 ambientLight;

void main()
{
	vec3 lightVectorWorld = lightPositionWorld - vertexPositionWorld;
	float diffuseBrightness = clamp(dot(normalize(lightVectorWorld), normalize(normalWorld)), 0, 1);
	vec4 diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1.0);

	daColor = diffuseLight + ambientLight;
}