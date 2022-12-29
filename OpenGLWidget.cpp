#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
{
	
}

OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::initializeGL()
{
	m_UIcontrol.m_Shader.m_f = QOpenGLContext::currentContext()->functions();   // represents a native OpenGL context, enabling OpenGL rendering on a QSurface.
	m_UIcontrol.m_Shader.mf = QOpenGLContext::currentContext()->extraFunctions();
	m_UIcontrol.m_Shader.InitializeEnable();
	m_UIcontrol.m_Shader.AddNewProgram("./Shaders/BlackBody.vert","./Shaders/BlackBody.frag");
	m_UIcontrol.m_Shader.m_square_quad.LoadData();
	m_UIcontrol.m_Shader.BindVAO_quad();
	m_UIcontrol.UpdateTexture();
}

void OpenGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);

	m_UIcontrol.m_Proj.setToIdentity();
	//m_UIcontrol.m_Proj.perspective(45.0f, GLfloat(w) / h, 0.1f, 50.0f);
	m_UIcontrol.m_Proj.ortho( -1.0f, 1.0f, -1.0f, 1.0f, -5.0f, 5.0f);
}

void OpenGLWidget::paintGL()
{
	m_UIcontrol.Draw();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton)
	{
		m_UIcontrol.m_CursorX = event->x();
		m_UIcontrol.m_CursorY = event->y();
	}
}

glm::vec3 OpenGLWidget::get_arcball_vector(double x, double y, int width, int height)
{
	glm::vec3 coord = glm::vec3(1.0 * x / width * 2 - 1.0,
		1.0 * y / height * 2 - 1.0, 0);
	coord.y = -coord.y;
	//coord.x = -coord.x;
	float length_squared = coord.x * coord.x + coord.y * coord.y;
	if (length_squared <= 1.0)
		coord.z = sqrt(1.0 - length_squared);
	else
		coord = glm::normalize(coord);
	return coord;
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		if (m_UIcontrol.m_ifDrag3D == false)
		{
			m_UIcontrol.m_Cursor.setShape(Qt::ClosedHandCursor);
			QMatrix4x4 modelUse = m_UIcontrol.m_MouseRotate;

			glm::vec3 prevPos = get_arcball_vector(m_UIcontrol.m_CursorX, m_UIcontrol.m_CursorY, this->width(), this->height());
			glm::vec3  currPos = get_arcball_vector(event->x(), event->y(), this->width(), this->height());
			float angle = acos(std::min(1.0f, glm::dot(prevPos, currPos)));
			glm::vec3 camAxis = -glm::cross(prevPos, currPos);
			glm::mat4 viewRotation = glm::rotate(glm::degrees(angle) * 0.01f, camAxis);

			m_UIcontrol.m_MouseRotate.setToIdentity();
			m_UIcontrol.m_MouseRotate = QMatrix4x4(viewRotation[0][0], viewRotation[0][1], viewRotation[0][2], viewRotation[0][3],
				viewRotation[1][0], viewRotation[1][1], viewRotation[1][2], viewRotation[1][3],
				viewRotation[2][0], viewRotation[2][1], viewRotation[2][2], viewRotation[2][3],
				viewRotation[3][0], viewRotation[3][1], viewRotation[3][2], viewRotation[3][3]);
			m_UIcontrol.m_MouseRotate = m_UIcontrol.m_MouseRotate * modelUse;

			m_UIcontrol.m_CursorX = event->x();
			m_UIcontrol.m_CursorY = event->y();

			update();
		}
		else
		{
			m_UIcontrol.m_Cursor.setShape(Qt::ClosedHandCursor);
			setCursor(m_UIcontrol.m_Cursor);

			float dragspeed = (1 / m_UIcontrol.m_ZoomFactor);

			float moveX = (event->x() - m_UIcontrol.m_CursorX) / (float)this->width() * dragspeed;
			float moveY = (event->y() - m_UIcontrol.m_CursorY) / (float)this->height() * dragspeed;

			m_UIcontrol.m_X += moveX; 
			m_UIcontrol.m_Y += - moveY;

			m_UIcontrol.m_CursorX = event->x();
			m_UIcontrol.m_CursorY = event->y();
			update();
		}

	}

}

void OpenGLWidget::wheelEvent(QWheelEvent* event)  
{

	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15; // see QWheelEvent documentation
	m_UIcontrol.m_ZoomFactor += (float)numSteps / 50.0f;
	update();

}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	//Set the cursor shape
	m_UIcontrol.m_Cursor.setShape(Qt::ArrowCursor);
	setCursor(m_UIcontrol.m_Cursor);
}


void OpenGLWidget::IfDrag3D(bool b)
{
	m_UIcontrol.m_ifDrag3D = b;
	update();
}

void OpenGLWidget::Relocate()
{
	m_UIcontrol.ResetMatrix();

	//emit XRotationChanged(0);
	//emit YRotationChanged(0);
	//emit ZRotationChanged(0);


	update();
}

void OpenGLWidget::SetLightX(int i)
{
	m_UIcontrol.m_LightX = i;
	update();
}
void OpenGLWidget::SetLightY(int i)
{
	m_UIcontrol.m_LightY = i;
	update();
}
void OpenGLWidget::SetLightZ(int i)
{
	m_UIcontrol.m_LightZ = i;
	update();
}
void OpenGLWidget::SetCrackDepth(int i)
{
	m_UIcontrol.m_CrackDepth = i;
	m_UIcontrol.UpdateTexture();
	update();
}
void OpenGLWidget::SetCrackWidth(int i)
{
	m_UIcontrol.m_CrackWidth = i;
	m_UIcontrol.UpdateTexture();
	update();
}
void OpenGLWidget::SetAmbient(int i)
{
	m_UIcontrol.m_Light_Ambient = i;
	update();
}
void OpenGLWidget::SetDiffuse(int i)
{
	m_UIcontrol.m_Light_Diffusion = i;
	update();
}
void OpenGLWidget::SetSpecular(int i)
{
	m_UIcontrol.m_Light_Specular = i;
	update();
}
void OpenGLWidget::SetShininess(int i)
{
	m_UIcontrol.m_Light_Shininess = i;
	update();
}
void OpenGLWidget::SetRoughness(int i)
{
	m_UIcontrol.m_Light_Roughness = i;
	update();
}