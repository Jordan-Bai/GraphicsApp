#pragma once
#include "GraphicsHeaders.h"
#include "glm.hpp"
#include "Utilities.h"
#include <vector>

class Mesh
{
protected:
	GLuint m_vertArray; // Sometimes called VAO (Vertex Array Object)
	GLuint m_vertBuffer; // Sometimes called VBO (Vertex Buffer Object)
	GLuint m_indexBuffer; // Sometimes called IBO (Index Buffer Object)

	int m_vertCount;
	int m_triCount;

public:
	Mesh()
		: m_vertArray(), m_vertBuffer(), m_indexBuffer()
	{
	}
	~Mesh();

	void CreateCubeMesh();
	void LoadFromFile(std::string fileName);
	//void InitObject();
	void InitObject(std::vector<Vertex>& verts, std::vector<int>& indicies);

	//glm::mat4 GetObjectSpace();

	virtual void Draw();
};