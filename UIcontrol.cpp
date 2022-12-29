#include "UIcontrol.h"

void UIcontrol::SetOpenGLfunction(QOpenGLFunctions* m_f, QOpenGLExtraFunctions* mf)
{
	m_Shader.m_f = m_f;
	m_Shader.mf = mf;
}

void UIcontrol::Draw()
{
	m_Shader.mf->glClearColor(0.3, 0.3, 1, 1);
	m_Shader.mf->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //| GL_STENCIL_BUFFER_BIT
	m_Shader.mf->glEnable(GL_DEPTH_TEST);
	
	m_Camera.setToIdentity();
	m_Camera.scale(m_ZoomFactor, m_ZoomFactor);
	m_World.setToIdentity();
	m_World.translate(m_X, m_Y);
	m_World *= m_MouseRotate;
	m_Shader.DrawVAO_quad(m_Proj, m_Camera, m_World, m_LightX, m_LightY, m_LightZ,
		m_CrackDepth, m_CrackWidth, m_Light_Ambient, m_Light_Diffusion, m_Light_Specular, m_Light_Shininess, m_Light_Roughness);
}

void UIcontrol::ResetMatrix()
{
	m_XRot = 0;
	m_YRot = 0;
	m_ZRot = 0;
	m_X = 0;
	m_Y = 0;
	m_Sx = 1;
	m_Sy = 1;
	m_ZoomFactor = 1;
	m_Eye = QVector3D(0, 0, 5);
	m_MouseRotate = QMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

void UIcontrol::UpdateTexture()
{
	float crackdepth = (float)m_CrackDepth;
	float crackwidth = (float)m_CrackWidth;

	m_CrackTexture.CreateCrack(crackwidth, crackdepth);
}