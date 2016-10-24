#version 430

out vec4 daColor;
in vec3 vertexPositionWorld;
in vec2 theUV;
in mat3 TBN;

uniform vec3 eyePositionWorld;
uniform vec3 lightPositionWorld;
uniform float ambientBrightness;
uniform sampler2D myTexture;
uniform sampler2D normalMap;

uniform float k0;
uniform float k1;
uniform float k2;

void main()
{
	vec3 normalWorld = texture(normalMap, theUV).rgb;
	normalWorld = normalize(normalWorld * 2.0 - 1.0);
	normalWorld = normalize(TBN * normalWorld);
	
	vec4 ambientLight = vec4(0.0, 0.0, ambientBrightness, 1.0);
	
	vec3 lightVectorWorld = lightPositionWorld - vertexPositionWorld;
	float distance = length(lightVectorWorld);
	float diffuseBrightness = clamp(dot(normalize(lightVectorWorld), normalize(normalWorld)) / (k0 + k1 * distance + k2 * distance * distance), 0, 1);
	vec4 diffuseLight = vec4(0.0, diffuseBrightness, 0.0, 1.0);

	vec3 eyeVectorWorld = eyePositionWorld - vertexPositionWorld;
	vec3 lightVectorReflect = reflect(-lightVectorWorld, normalize(normalWorld));
	float specularBrightness = clamp(dot(normalize(lightVectorReflect), normalize(eyeVectorWorld)), 0, 1);
	pow(specularBrightness, 100);
	vec4 specularLight = vec4(specularBrightness, 0.0, 0.0, 1.0);

	daColor = texture(myTexture, theUV) * clamp(ambientLight + diffuseLight + specularLight, vec4(0.0, 0.0, 0.0, 1.0), vec4(1.0, 1.0, 1.0, 1.0));
}