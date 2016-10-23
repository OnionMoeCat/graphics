#version 430

in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normalModel;
in layout(location=3) vec2 uv;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 modelToWorldInvertTrans;

out vec3 normalWorld;
out vec3 vertexPositionWorld;
out vec2 theUV;

void main()
{
	gl_Position = modelToProjectionMatrix * vec4(vertexPositionModel, 1);
	normalWorld = vec3(modelToWorldInvertTrans * vec4(normalModel, 0));
	vertexPositionWorld = vec3(modelToWorldMatrix * vec4(vertexPositionModel, 1));
	theUV = uv;
}