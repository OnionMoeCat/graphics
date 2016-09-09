const char* vertexShaderCode =
	"#version 430\r\n"
	"in layout(location = 0) vec2 position;"
	"in layout(location = 1) vec3 vertexColor;"
	""
	"out vec3 theColor;"
	"uniform mat4 transform;"
	"void main()"
	"{"
	"	vec4 positionVec4 = vec4(position, 0.0f, 1.0f);"
	"	gl_Position = transform * positionVec4;"
	"	theColor = vertexColor;"
	"}";

const char* fragmentShaderCode =
	"#version 430\r\n"
	"in vec3 theColor;"
	""
	"out vec4 daColor;"
	""
	"void main()"
	"{"
	"	daColor = vec4(theColor, 1.0);"
	"}";