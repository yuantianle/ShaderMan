#pragma once
#include "Lib.h"

struct PVertex
{
	QVector3D position;
	QVector3D color;
	QVector2D texcoord;
	QVector3D normal;
};

struct SimpleVertex
{
	QVector3D position;
	QVector3D color;
};

struct TempVertex
{
	QVector3D position;
	QVector3D color;
	QVector2D texcoord;
};

class TriGrid
{
public:
	QVector<PVertex> m_Vertices; //for saving the vertices
	QVector<GLuint> m_Indices;  //for saving the indices

public:
	TriGrid() {}
	~TriGrid() {}

public:

	void LoadData(std::vector<glm::vec2> point_garage, std::vector<glm::vec3> point_color_garage)  // { [cell1->vertices], [cell2->vertices], ... }
	{
		m_Vertices.clear();
		m_Indices.clear();

		for (int i = 0; i < point_garage.size(); i++)
		{
				PVertex Vert = { QVector3D(point_garage[i].x ,point_garage[i].y, 0.f),
					QVector3D(point_color_garage[i].x, point_color_garage[i].y, point_color_garage[i].z),
					QVector2D(0.f, 1.f), QVector3D(0.0f, 0.f, 1.f) };
				m_Vertices.push_back(Vert);
		}

		for (int i = 0; i < point_garage.size() - 1; i+=2)
		{
				m_Indices.push_back(i);
				m_Indices.push_back(i + 1);
		}
	}
	void LoadData()
	{
		m_Vertices.push_back({ QVector3D(-0.5f, 0.5f, 0.0f), QVector3D(0.0f, 0.f, 1.f), QVector2D(0.f, 1.f),   QVector3D(0.0f, 0.f, 1.f) });
		m_Vertices.push_back({ QVector3D(-0.5f, -0.5f, 0.0f), QVector3D(1.0f, 1.f, 1.f), QVector2D(0.f, 0.f), QVector3D(0.0f, 0.f, 1.f) });
		m_Vertices.push_back({ QVector3D(0.5f, -0.5f, 0.0f), QVector3D(1.0f, 1.f, 1.f), QVector2D(1.f, 0.f), QVector3D(0.0f, 0.f, 1.f) });
		m_Vertices.push_back({ QVector3D(0.5f, 0.5f, 0.0f), QVector3D(1.0f, 0.f, 1.f), QVector2D(1.f, 1.f), QVector3D(0.0f, 0.f, 1.f) });

		m_Indices.push_back(0);
		m_Indices.push_back(1);
		m_Indices.push_back(2);
		m_Indices.push_back(3);

	}

	inline PVertex* GetVertices() { return m_Vertices.data(); }

	inline const unsigned GetNrOfVertices() { return m_Vertices.size(); }
	inline GLuint* GetIndices() { return m_Indices.data(); }
	inline const unsigned GetNrOfIndices() { return m_Indices.size(); }
};

class TriSolid
{
public:
	QVector<PVertex> m_Vertices; //for saving the vertices
	QVector<GLuint> m_Indices;  //for saving the indices

public:
	TriSolid() {}
	~TriSolid() {}

public:
	void LoadData(std::vector<glm::vec2> point_garage, std::vector<glm::vec3> point_color_garage)  // { [cell1->vertices], [cell2->vertices], ... }
	{
		m_Vertices.clear();
		m_Indices.clear();

		for (int i = 0; i < point_garage.size(); i++)
		{
			PVertex Vert = { QVector3D(point_garage[i].x ,point_garage[i].y, 0.f),
				QVector3D(point_color_garage[i].x, point_color_garage[i].y, point_color_garage[i].z),
				QVector2D(0.f, 1.f), QVector3D(0.0f, 0.f, 1.f) };
			m_Vertices.push_back(Vert);
		}

		for (int i = 0; i < point_garage.size() - 2; i += 3)
		{
			m_Indices.push_back(i);
			m_Indices.push_back(i + 1);
			m_Indices.push_back(i + 2);
		}

	}
	void LoadData()
	{
		m_Vertices.push_back({ QVector3D(-0.5f, 0.5f, 0.0f), QVector3D(0.0f, 0.f, 1.f), QVector2D(0.f, 1.f),   QVector3D(0.0f, 0.f, 1.f) });
		m_Vertices.push_back({ QVector3D(-0.5f, -0.5f, 0.0f), QVector3D(1.0f, 1.f, 1.f), QVector2D(0.f, 0.f), QVector3D(0.0f, 0.f, 1.f) });
		m_Vertices.push_back({ QVector3D(0.5f, -0.5f, 0.0f), QVector3D(1.0f, 1.f, 1.f), QVector2D(1.f, 0.f), QVector3D(0.0f, 0.f, 1.f) });
		m_Vertices.push_back({ QVector3D(0.5f, 0.5f, 0.0f), QVector3D(1.0f, 0.f, 1.f), QVector2D(1.f, 1.f), QVector3D(0.0f, 0.f, 1.f) });

		m_Indices.push_back(0);
		m_Indices.push_back(1);
		m_Indices.push_back(2);
		m_Indices.push_back(3);

	}

	inline PVertex* GetVertices() { return m_Vertices.data(); }

	inline const unsigned GetNrOfVertices() { return m_Vertices.size(); }
	inline GLuint* GetIndices() { return m_Indices.data(); }
	inline const unsigned GetNrOfIndices() { return m_Indices.size(); }
};
//QVector3D(-0.5f, 0.5f, 0.0f), QVector3D(0.0f, 0.f, 1.f), QVector2D(0.f, 1.f), QVector3D(0.0f, 0.f, 1.f),
//QVector3D(-0.5f, -0.5f, 0.0f), QVector3D(1.0f, 1.f, 1.f), QVector2D(0.f, 0.f), QVector3D(0.0f, 0.f, 1.f),
//QVector3D(0.5f, -0.5f, 0.0f), QVector3D(1.0f, 1.f, 1.f), QVector2D(1.f, 0.f), QVector3D(0.0f, 0.f, 1.f),
//QVector3D(0.5f, 0.5f, 0.0f), QVector3D(1.0f, 0.f, 1.f), QVector2D(1.f, 1.f), QVector3D(0.0f, 0.f, 1.f)
