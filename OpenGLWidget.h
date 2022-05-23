#pragma once
#include "Lib.h"
#include "UIcontrol.h"

class OpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	OpenGLWidget(QWidget* parent = Q_NULLPTR);
	~OpenGLWidget();

public:
	UIcontrol m_UIcontrol;

protected:
	void initializeGL();            // Initialization
	void resizeGL(int w, int h);    // Reshape
	void paintGL();                 // Display

public:
	glm::vec3 get_arcball_vector(double x, double y, int width, int height);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void wheelEvent(QWheelEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

};
