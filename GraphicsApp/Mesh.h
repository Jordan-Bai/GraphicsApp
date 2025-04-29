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

	int m_triCount;

public:
	Mesh();
	~Mesh();

	void CreatePlaneMesh();
	void CreateCubeMesh();
	void LoadFromFile(std::string fileName);
	virtual void InitObject(std::vector<Vertex>& verts, std::vector<int>& indicies);

	virtual void Draw();
};