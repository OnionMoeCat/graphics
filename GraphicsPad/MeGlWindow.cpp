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
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 11;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint programWithoutLightID;
GLuint cubeNumIndices;
Camera camera;
GLuint fullTransformationUniformLocation;

GLuint theBufferID;

GLuint cubeVertexArrayObjectID;
GLuint cubeIndexByteOffset;

glm::vec3 cube1PositionWorld(0.0f, 2.0f, 0.0f);
glm::vec3 cube2PositionWorld(-2.0f, 2.0f, -2.0f);

glm::vec3 lightPositionWorld(0.0f, 4.0f, 0.0f);

GLuint onionmoecatTextureObjectId;
GLuint defaultTextureObjectId;

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
	GLuint planeByteOffset = 0;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 6));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 9));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, theBufferID);

}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	mat4 modelToProjectionMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	glUseProgram(programID);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, onionmoecatTextureObjectId);

	GLint myTextureLocation = glGetUniformLocation(programID, "myTexture");
	if (myTextureLocation > 0) {
		glUniform1i(myTextureLocation, 0);
	}

	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientBrightness");

	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	glUniform3fv(lightPositionUniformLocation, 1, &lightPositionWorld[0]);

	GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	glUniform3fv(eyePositionUniformLocation, 1, &camera.getPosition()[0]);

	float r = 10.0f;

	GLint k0UniformLocation = glGetUniformLocation(programID, "k0");
	float k0 = 1.0f;
	glUniform1f(k0UniformLocation, k0);

	GLint k1UniformLocation = glGetUniformLocation(programID, "k1");
	float k1 = 2 / r;
	glUniform1f(k1UniformLocation, k1);

	GLint k2UniformLocation = glGetUniformLocation(programID, "k2");
	float k2 = 1 / r / r;
	glUniform1f(k2UniformLocation, k2);

	GLint modelToWorldMatrixUniformLocation =
		glGetUniformLocation(programID, "modelToWorldMatrix");
	GLint modelToWorldInvertTransUniformLocation =
		glGetUniformLocation(programID, "modelToWorldInvertTrans");

	GLint fullTransformationUniformLocation = glGetUniformLocation(programID, "modelToProjectionMatrix");

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	mat4 cubeModelToWorldMatrix = glm::translate(cube1PositionWorld);
	mat4 cubeModelToWorldInverseTrans = glm::inverse(glm::transpose(cubeModelToWorldMatrix));
	modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&cubeModelToWorldMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldInvertTransUniformLocation, 1, GL_FALSE,
		&cubeModelToWorldInverseTrans[0][0]);
	float ambientLight = 0.1f;
	glUniform1f(ambientLightUniformLocation, ambientLight);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	glBindTexture(GL_TEXTURE_2D, defaultTextureObjectId);

	if (myTextureLocation > 0) {
		glUniform1i(myTextureLocation, 0);
	}

	glBindVertexArray(cubeVertexArrayObjectID);
	cubeModelToWorldMatrix = glm::scale(15.0f, 0.01f, 15.0f);
	cubeModelToWorldInverseTrans = glm::inverse(glm::transpose(cubeModelToWorldMatrix));
	modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE,
		&cubeModelToWorldMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldInvertTransUniformLocation, 1, GL_FALSE,
		&cubeModelToWorldInverseTrans[0][0]);
	ambientLight = 0.1f;
	glUniform1f(ambientLightUniformLocation, ambientLight);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	glUseProgram(programWithoutLightID);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, defaultTextureObjectId);

	GLint myTextureLocationWithoutLight = glGetUniformLocation(programWithoutLightID, "myTexture");
	if (myTextureLocationWithoutLight > 0) {
		glUniform1i(myTextureLocationWithoutLight, 0);
	}

	GLint modelToWorldMatrixUniformLocationWithoutLight =
		glGetUniformLocation(programWithoutLightID, "modelToWorldMatrix");
	GLint fullTransformationUniformLocationWithoutLight = glGetUniformLocation(programWithoutLightID, "modelToProjectionMatrix");

	glBindVertexArray(cubeVertexArrayObjectID);
	cubeModelToWorldMatrix = glm::translate(lightPositionWorld) * glm::scale(0.1f, 0.1f, 0.1f);
	modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocationWithoutLight, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocationWithoutLight, 1, GL_FALSE,
		&cubeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);
	
	glBindTexture(GL_TEXTURE_2D, onionmoecatTextureObjectId);

	if (myTextureLocationWithoutLight > 0) {
		glUniform1i(myTextureLocationWithoutLight, 0);
	}

	//glUseProgram(programWithoutLightID);
	glBindVertexArray(cubeVertexArrayObjectID);
	cubeModelToWorldMatrix = glm::translate(cube2PositionWorld);
	modelToProjectionMatrix = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformationUniformLocationWithoutLight, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocationWithoutLight, 1, GL_FALSE,
		&cubeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeNumIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	if (glGetError() != GL_NO_ERROR)
		return;
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
	GLuint fragmentShaderWithoutLightID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderWithoutLightCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderWithoutLightID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);
	glCompileShader(fragmentShaderWithoutLightID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID) || !checkShaderStatus(fragmentShaderWithoutLightID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	programWithoutLightID = glCreateProgram();
	glAttachShader(programWithoutLightID, vertexShaderID);
	glAttachShader(programWithoutLightID, fragmentShaderWithoutLightID);
	glLinkProgram(programWithoutLightID);

	if (!checkProgramStatus(programID) || !checkProgramStatus(programWithoutLightID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteShader(fragmentShaderWithoutLightID);
}

void MeGlWindow::initializeTextures() {
	const char* texName = "onionmoecat.png";
	QImage temp = QImage(texName, "PNG");
	if (temp.isNull()) {
		return;
	}
	QImage timg = QGLWidget::convertToGLFormat(temp);

	glGenTextures(1, &onionmoecatTextureObjectId);
	glBindTexture(GL_TEXTURE_2D, onionmoecatTextureObjectId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(), timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, timg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	texName = "default.png";
	temp = QImage(texName, "PNG");
	if (temp.isNull()) {
		return;
	}
	timg = QGLWidget::convertToGLFormat(temp);

	glGenTextures(1, &defaultTextureObjectId);
	glBindTexture(GL_TEXTURE_2D, defaultTextureObjectId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, timg.width(), timg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, timg.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (glGetError() != GL_NO_ERROR)
		return;
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
}

MeGlWindow::~MeGlWindow()
{
	glDeleteBuffers(1, &theBufferID);
	glUseProgram(0);
	glDeleteProgram(programID);
}