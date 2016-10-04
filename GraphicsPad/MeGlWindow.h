#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <gl\glew.h>
#include <QtOpenGL\qglwidget>
#include <string>
#include <Qt\qtimer.h>

class MeGlWindow : public QGLWidget
{
	Q_OBJECT
	void sendDataToOpenGL();
	bool checkShaderStatus(GLuint shaderID);
	bool checkProgramStatus(GLuint programID);
	bool checkStatus(
		GLuint objectID,
		PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
		PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
		GLenum statusType);
	std::string readShaderCode(const char* fileName);
	void installShaders();
	QTimer* qTimer;
protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
public:
	~MeGlWindow();
	MeGlWindow();
private slots:
	void update();
};

#endif
