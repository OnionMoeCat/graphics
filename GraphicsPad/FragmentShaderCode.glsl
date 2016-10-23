#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;
in vec2 theUV;

uniform vec3 eyePositionWorld;
uniform vec3 lightPositionWorld;
uniform float ambientBrightness;
uniform sampler2D myTexture;

uniform float k0;
uniform float k1;
uniform float k2;

void main()
{
	vec4 ambientLight = vec4(0.0, 0.0, ambientBrightness, 1.0);
	
	vec3 lightVectorWorld = lightPositionWorld - vertexPositionWorld;
	float distance = length(lightVectorWorld);
	float diffuseBrightness = clamp(dot(normalize(lightVectorWorld), normalize(normalWorld)) / (k0 + k1 * distance + k2 * distance * distance), 0, 1);
	vec4 diffuseLight = vec4(0.0, diffuseBrightness, 0.0, 1.0);

	vec3 eyeVectorWorld = eyePositionWorld - vertexPositionWorld;
	vec3 lightVectorReflect = reflect(-lightVectorWorld, normalize(normalWorld));
	float specularBrightness = clamp(dot(normalize(lightVectorReflect), normalize(eyeVectorWorld)), 0, 1);
	pow(specularBrightness, 50);
	vec4 specularLight = vec4(specularBrightness, 0.0, 0.0, 1.0);

	daColor = texture(myTexture, theUV) * (ambientLight + diffuseLight + specularLight);
}