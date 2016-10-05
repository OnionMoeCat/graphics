#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;
in vec2 theUV;

uniform vec3 lightPositionWorld;
uniform float ambientBrightness;
uniform sampler2D myTexture;

void main()
{
	vec3 lightVectorWorld = lightPositionWorld - vertexPositionWorld;
	float diffuseBrightness = clamp(dot(normalize(lightVectorWorld), normalize(normalWorld)), 0, 1);
	vec4 diffuseLight = vec4(diffuseBrightness, diffuseBrightness, diffuseBrightness, 1);
	vec4 ambientLight = vec4(ambientBrightness, ambientBrightness, ambientBrightness, 1);
	//daColor = texture(myTexture, theUV) * (diffuseLight + ambientLight);
	daColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}