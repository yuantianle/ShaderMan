#pragma once
#include "Lib.h"
#include "Shader.h"

class UIcontrol
{
public:
	UIcontrol() {}
	~UIcontrol() {}

public:
	Shader m_Shader;
	QOpenGLFunctions* m_f;
	QOpenGLExtraFunctions* mf;
	QCursor m_Cursor;

public:
	QVector3D m_Eye = QVector3D(0, 0, 25);
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

public:
	bool m_ifDrag3D = true;

public:
	void InitializeEnable();
};