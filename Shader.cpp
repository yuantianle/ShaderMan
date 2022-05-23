#include "Shader.h"

void Shader::AddNewProgram(QString s1, QString s2)
{
	QOpenGLShaderProgram* Program = new QOpenGLShaderProgram;
	Program->addShaderFromSourceFile(QOpenGLShader::Vertex,
		s1);
	Program->addShaderFromSourceFile(QOpenGLShader::Fragment,
		s2);
	m_Programs.push_back(Program);
}