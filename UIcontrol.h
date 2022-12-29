#pragma once
#include "Lib.h"
#include "Shader.h"
#include "GenTexture.h"

class UIcontrol
{
public:
	UIcontrol() {}
	~UIcontrol() {}

public:
	Shader m_Shader;
	QCursor m_Cursor;
	GenTexture m_CrackTexture;

public:
	QVector3D m_Eye = QVector3D(0, 0, 5);
	QVector3D m_Object = QVector3D(0, 0, 0);
	QMatrix4x4 m_Proj = QMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	QMatrix4x4 m_World = QMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1); // the model matrix
	QMatrix4x4 m_Camera = QMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);  // the view matrix
	float m_XRot = 0;
	float m_YRot = 0;
	float m_ZRot = 0;
	float m_X = 0;
	float m_Sx = 1;
	float m_Sy = 1;  // scale, we dont use y
	float m_ZoomFactor = 1;
	float m_Y = 0;
	QMatrix4x4 m_MouseRotate = QMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	float m_CursorX;
	float m_CursorY;
	float m_middleCursorX = 0;
	float m_middleCursorY = 0;

	int m_LightX = 0;  //change light in x direction
	int m_LightY = 0;
	int m_LightZ = 20;

	int m_CrackDepth = 0;  //based on texture information
	int m_CrackWidth = 0;

	int m_Light_Ambient = 0;
	int m_Light_Diffusion = 0;
	int m_Light_Specular = 0;
	int m_Light_Shininess = 5;
	int m_Light_Roughness = 0;

public:
	bool m_ifDrag3D = false;

public:
	void SetOpenGLfunction(QOpenGLFunctions* m_f, QOpenGLExtraFunctions* mf);
	void Draw();
	void ResetMatrix();
	void UpdateTexture();
};