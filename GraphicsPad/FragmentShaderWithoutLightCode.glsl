#version 430

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;
in vec2 theUV;

uniform sampler2D myTexture;

void main()
{
	daColor = texture(myTexture, theUV);
}