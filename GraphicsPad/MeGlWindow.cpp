#include <gl\glew.h>
#include <MeGlWindow.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <QtGui\qkeyevent>

#include <exception>

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;

namespace 
{
	float g_translate_x = 0.0f;
	float g_translate_y = 0.0f;
	float g_translate_z = 0.0f;

	float g_rotation_x = 0.0f;
	float g_rotation_y = 0.0f;
	float g_rotation_z = 0.0f;

	GLint scaleUniformLocation = 0;

	struct sVertex
	{
		float x, y;
		float r, g, b;
	};

	void updateNormal();
	void sendDataToOpenGL();
	void installShaders();
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	float SPEED = 0.02f;
	float ANGULARSPEED = 2.0f;
	switch (e->key())
	{
	case Qt::Key::Key_W:
		g_translate_y += SPEED;
		break;
	case Qt::Key::Key_S:
		g_translate_y -= SPEED;
		break;
	case Qt::Key::Key_A:
		g_translate_x -= SPEED;
		break;
	case Qt::Key::Key_D:
		g_translate_x += SPEED;
		break;
	case Qt::Key::Key_Q:
		g_rotation_z += ANGULARSPEED;
		break;
	case Qt::Key::Key_E:
		g_rotation_z -= ANGULARSPEED;
		break;
	default:
		break;
	}

	updateNormal();
	repaint();
}

void MeGlWindow::initializeGL()
{	
	glewInit();
	sendDataToOpenGL();
	installShaders();
	updateNormal();
}

void MeGlWindow::paintGL()
{
	glViewport(0, 0, width(), height());
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

namespace 
{
	void sendDataToOpenGL()
	{
		GLfloat vertices[] =
		{
			0.0f, 0.1f,
			1.0f, 0.0f, 0.0f,
			-0.1f, -0.1f,
			0.0f, 1.0f, 0.0f,
			0.1f, -0.1f,
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

		scaleUniformLocation = glGetUniformLocation(programID, "transform");
	}

	void updateNormal()
	{
		glm::mat4 scaleMatrix;
		glm::mat4 translateMatrix = glm::translate(g_translate_x, g_translate_y, g_translate_z);
		glm::mat4 rotationMatrix = glm::rotate(g_rotation_z, 0.0f, 0.0f, 1.0f);

		glm::mat4 transformMatrix = translateMatrix * rotationMatrix * scaleMatrix;

		glUniformMatrix4fv(scaleUniformLocation, 1, GL_FALSE, &transformMatrix[0][0]);
	}
}

