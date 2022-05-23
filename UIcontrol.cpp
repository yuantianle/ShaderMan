#include "UIcontrol.h"

void UIcontrol::InitializeEnable()
{
	m_f->glClearColor(0.5, 0.5, 0.5, 1);  //init() part
	m_f->glEnable(GL_DEPTH_TEST);
	m_f->glFrontFace(GL_CCW);
	m_f->glEnable(GL_BLEND);
	m_f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_f->glEnable(GL_LIGHTING);
	m_f->glEnable(GL_LIGHT0);
	m_f->glEnable(GL_COLOR_MATERIAL);
	m_f->glEnable(GL_TEXTURE_2D);
	m_f->glEnable(GL_TEXTURE0);
}
