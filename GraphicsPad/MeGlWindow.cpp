#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>
#include <MeGlWindow.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <Primitives\Vertex.h>
#include <Primitives\ShapeGenerator.h>
#include "Camera.h"
using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 11;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint cubeNumIndices;
Camera camera;
GLuint fullTransformationUniformLocation;

GLuint theBufferID;

GLuint cubeVertexArrayObjectID;
GLuint cubeIndexByteOffset;

glm::vec3 lightPositionWorld(0.0f, 5.0f, 0.0f);

void MeGlWindow::sendDataToOpenGL()
{
	ShapeData cube = ShapeGenerator::makeCube();

	glGenBuffers(1, &theBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	glBufferData(GL_ARRAY_BUFFER, 
		cube.vertexBufferSize() + cube.indexBufferSize(), 0, GL_STATIC_DRAW);
	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	cubeIndexByteOffset = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();

	cubeNumIndices = cube.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, theBufferID);
	GLuint cubeByteOffset = 0;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)cubeByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(cubeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(cubeByteOffset + sizeof(float) * 6));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(cubeByteOffset + sizeof(float) * 9));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

	cube.cleanup();
}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	mat4 modelToProjectionMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientBrightness");

	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	glUniform3fv(lightPositionUniformLocation, 1, &lightPositionWorld[0]);

	GLint modelToWorldMatrixUniformLocation =
		glGetUniformLocation(programID, "modelToWorldMatrix");

	// Plane
	glBindVertexArray(planeVertexArrayObjectID);
	mat4 planeModelToWorldMatrix;
	modelToProjectionMatrix = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&planeModelToWorldMatrix[0][0]);
	float ambientLight = 0.1f;
	glUniform1f(ambientLightUniformLocation, ambientLight);
	glDrawElements(GL_TRIANGLES, planeNumIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);

	// Plane
	glBindVertexArray(planeVertexArrayObjectIDHack);
	mat4 planeModelToWorldMatrixHack = glm::translate(-2.5f, 0.0f, -2.5f);
	modelToProjectionMatrix = worldToProjectionMatrix * planeModelToWorldMatrixHack;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&planeModelToWorldMatrixHack[0][0]);
	ambientLight = 1.0f;
	glUniform1f(ambientLightUniformLocation, ambientLight);
	glDrawElements(GL_TRIANGLES, planeNumIndicesHack, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffsetHack);

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	mat4 cubeModelToWorldMatrix = glm::translate(lightPositionWorld) * glm::scale(0.1f, 0.1f, 0.1f);
	modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&cubeModelToWorldMatrix[0][0]);
	ambientLight = 1.0f;
	glUniform1f(ambientLightUniformLocation, ambientLight);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

}

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	const float LIGHTMOVEMENTSPEED = 0.1f;
	switch (e->key())
	{
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	case Qt::Key::Key_Up:
		lightPositionWorld.z -= LIGHTMOVEMENTSPEED;
		break;
	case Qt::Key::Key_Down:
		lightPositionWorld.z += LIGHTMOVEMENTSPEED;
		break;
	case Qt::Key::Key_Left:
		lightPositionWorld.x -= LIGHTMOVEMENTSPEED;
		break;
	case Qt::Key::Key_Right:
		lightPositionWorld.x += LIGHTMOVEMENTSPEED;
		break;
	case Qt::Key::Key_I:
		lightPositionWorld.y += LIGHTMOVEMENTSPEED;
		break;
	case Qt::Key::Key_K:
		lightPositionWorld.y -= LIGHTMOVEMENTSPEED;
		break;
	}
	repaint();
}

bool MeGlWindow::checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool MeGlWindow::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool MeGlWindow::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string MeGlWindow::readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void MeGlWindow::installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void MeGlWindow::initializeTextures() {
	const char* texName = "onionmoecat.png";
	QImage temp = QImage(texName, "PNG");
	if (temp.isNull()) {
		return;
	}
	QImage timg = QGLWidget::convertToGLFormat(temp);
	glActiveTexture(GL_TEXTURE0);

	GLuint myTextureObjectId;
	glGenTextures(1, &myTextureObjectId);
	glBindTexture(GL_TEXTURE_2D, myTextureObjectId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(), timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, timg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	int loc = glGetUniformLocation(programID, "myTexture");
	if (loc > 0) {
		glUniform1i(loc, 0);
	}
}

void MeGlWindow::initializeGL()
{
	setMouseTracking(true);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	sendDataToOpenGL();
	installShaders();
	initializeTextures();
	fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");
}

MeGlWindow::MeGlWindow() 
{
	qTimer = new QTimer(this);
	connect(qTimer, SIGNAL(timeout()), this, SLOT(update()));
	qTimer->start(0);
}

MeGlWindow::~MeGlWindow()
{
	delete qTimer;
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
}

void MeGlWindow::update() {
	const float ROTATIONSPEED = 0.1f;
	repaint();
}