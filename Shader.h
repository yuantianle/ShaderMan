#pragma once
#include "Lib.h"
#include "Primitives.h"

class Shader
{
public:
	Shader() {}
	~Shader() {}

public:
	QOpenGLFunctions* m_f;
	QOpenGLExtraFunctions* mf;
	std::vector<QOpenGLShaderProgram*> m_Programs;

public:
	void InitializeEnable();
	void AddNewProgram(QString s1, QString s2);

public:
	GLuint m_VAO_quad;
	GLuint m_VBO_quad;

public:
	TriGrid m_square_grid;
	TriSolid m_square_quad;

public:
	void BindVAO_quad();
	void DrawVAO_quad(QMatrix4x4 Proj, QMatrix4x4 Camera, QMatrix4x4 Whole, int LightX, int LightY, int LightZ, int CrackDepth, int CrackWidthint, int LAmbient, int LDiffuse, int LSpecular, int LShininess, int LRoughness);
};