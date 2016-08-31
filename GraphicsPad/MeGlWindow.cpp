#include <gl\glew.h>
#include <MeGlWindow.h>

#include <exception>

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;

struct sVertex
{
	float x, y;
	float r, g, b;
};

void sendDataToOpenGL()
{
	GLfloat vertices[] =
	{
		0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f,
		0.0f, 0.0f, 1.0f
	};
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLvoid* offset = 0;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(sVertex), offset);
	offset = reinterpret_cast<GLvoid*>(reinterpret_cast<uint8_t*>(offset) + 2 * sizeof(float));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(sVertex), offset);

	GLushort indices[] =
	{
		0, 1, 2
	};
	GLuint indiceBuffer;
	glGenBuffers(1, &indiceBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* vertexShaderSources[] =
	{
		reinterpret_cast<const GLchar*>(vertexShaderCode)
	};

	glShaderSource(vertexShaderID, 1, vertexShaderSources, 0);

	const GLchar* fragmentShaderSources[] =
	{
		reinterpret_cast<const GLchar*>(fragmentShaderCode)
	};

	glShaderSource(fragmentShaderID, 1, fragmentShaderSources, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	GLint success;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		throw std::exception();
	}

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		throw std::exception();
	}

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	glUseProgram(programID);
}

void MeGlWindow::initializeGL()
{
	glewInit();
	sendDataToOpenGL();
	installShaders();
}

void MeGlWindow::paintGL()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}