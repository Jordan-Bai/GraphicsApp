#pragma once
//#include "GraphicsHeaders.h"
//#include "glm.hpp"
#include "Utilities.h"
#include "Texture.h"
#include <vector>

class Mesh
{
protected:
	GLuint m_vertArray; // Sometimes called VAO (Vertex Array Object)
	GLuint m_vertBuffer; // Sometimes called VBO (Vertex Buffer Object)
	GLuint m_indexBuffer; // Sometimes called IBO (Index Buffer Object)

	int m_triCount;
	std::vector<Vertex> m_verts;
	std::vector<int> m_indicies;

public:
	Mesh();
	Mesh(const Mesh& other) = delete;
	~Mesh();

	void CreatePlaneMesh();
	void CreateCubeMesh();
	void CreateFromHeightMap(Texture* heightmap, int sizeX, int sizeZ);
	void CreateFromHeightMap(Texture* heightmap, int sizeX, int sizeZ, int vertsPerPixel);
	void LoadFromFile(std::string fileName);
	virtual void InitObject();

	virtual void Draw();

	Mesh& operator=(const Mesh& other) = delete;

	std::vector<Vertex> GetVerts();
};