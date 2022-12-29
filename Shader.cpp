#include "Shader.h"


void Shader::InitializeEnable()
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

void Shader::AddNewProgram(QString s1, QString s2)
{
	QOpenGLShaderProgram* Program = new QOpenGLShaderProgram;
	Program->addShaderFromSourceFile(QOpenGLShader::Vertex,
		s1);
	Program->addShaderFromSourceFile(QOpenGLShader::Fragment,
		s2);
	m_Programs.push_back(Program);
}

void Shader::BindVAO_quad()
{
	mf->glGenVertexArrays(1, &m_VAO_quad);
	//2.Bind VAO: activate the VAO and anything we do with any other buffers will bind to this VAO
	mf->glBindVertexArray(m_VAO_quad);
	//3.Generate VBO
	mf->glGenBuffers(1, &m_VBO_quad); //Generate a ID for the buffer
	//4.Bind VBO: store array data into the box
	mf->glBindBuffer(GL_ARRAY_BUFFER, m_VBO_quad);  //Store array of floats into one part of the box(VBO)
	//5.Send data to VBO
	mf->glBufferData(GL_ARRAY_BUFFER, m_square_quad.GetNrOfVertices() * sizeof(PVertex), m_square_quad.GetVertices(), GL_STATIC_DRAW);//Send the data in VBO box to the graphics card; GL_STATIC_DRAW: how often u wanna modify the data

	mf->glEnableVertexAttribArray(0);
	mf->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PVertex), (GLvoid*)offsetof(PVertex, position));
	//color
	mf->glEnableVertexAttribArray(1);
	mf->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PVertex), (GLvoid*)offsetof(PVertex, color));
	//texcoord
	mf->glEnableVertexAttribArray(2);
	mf->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PVertex), (GLvoid*)offsetof(PVertex, texcoord));
	//

	mf->glEnableVertexAttribArray(3);
	mf->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(PVertex), (GLvoid*)offsetof(PVertex, normal));

	mf->glBindBuffer(GL_ARRAY_BUFFER, 0);
	mf->glBindVertexArray(0);
}

void Shader::DrawVAO_quad(QMatrix4x4 Proj, QMatrix4x4 Camera, QMatrix4x4 Whole, int LightX, int LightY, int LightZ, 
	int CrackDepth, int CrackWidth, int LAmbient, int LDiffuse, int LSpecular, int LShininess, int LRoughness)
{
	m_Programs[0]->bind();
	m_Programs[0]->setUniformValue("ProjectionMatrix", Proj);
	m_Programs[0]->setUniformValue("ViewMatrix", Camera);
	m_Programs[0]->setUniformValue("ModelMatrix", Whole);
	m_Programs[0]->setUniformValue("LightPos0", QVector3D(0.1 * (float)LightX, 0.1 * (float)LightY, 0.1 * (float)LightZ));
	QImage Ivtd = QImage("./Texture/Pic1.png");
	QOpenGLTexture* TexturePattern = new QOpenGLTexture(Ivtd);
	if (TexturePattern != NULL) TexturePattern->bind();

	float lambient = LAmbient * 0.01;
	float ldiffuse = LDiffuse * 0.01;
	float lspecular = LSpecular * 0.01;
	float lshininess = LShininess;
	float lroughness = LRoughness * 0.01;

	m_Programs[0]->setUniformValue("uAmbient", lambient);    //0~1
	m_Programs[0]->setUniformValue("uDiffusion", ldiffuse);  //0~1
	m_Programs[0]->setUniformValue("uSpecular", lspecular);   //0~1
	m_Programs[0]->setUniformValue("uShininess", lshininess);  //1~100
	m_Programs[0]->setUniformValue("uRoughness", lroughness);  //0~1 

	m_Programs[0]->setUniformValue("CrackInfoTexture", GL_TEXTURE0 - GL_TEXTURE0);

	BindVAO_quad(); //cracks
	mf->glBindVertexArray(m_VAO_quad);

	mf->glDrawArrays(GL_QUADS, 0, 4);
	//m_f->glDrawArrays(GL_TRIANGLES, 0, m_square_quad.GetNrOfIndices());
	mf->glBindVertexArray(0);

	m_Programs[0]->release();
}