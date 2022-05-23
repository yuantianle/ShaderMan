#pragma once
#include "Lib.h"

class Shader
{
public:
	Shader() {}
	~Shader() {}

public:
	std::vector<QOpenGLShaderProgram*> m_Programs;

public:
	void AddNewProgram(QString s1, QString s2);
};