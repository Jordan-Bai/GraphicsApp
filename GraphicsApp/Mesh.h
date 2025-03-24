#pragma once
#include <vector>
#include "GraphicsHeaders.h"
#include "glm.hpp"
#include "Utilities.h"

class Mesh
{
protected:
	GLuint m_vertBuffer;
	glm::vec3 m_pos;
	std::vector<Vertex> m_verts; // Vertices stored in local space

public:
	Mesh()
		: m_pos(), m_vertBuffer()
	{
	}
	~Mesh();

	void LoadFromFile(std::string fileName);
	void CalculateTangents();
	void InitObject();

	glm::mat4 GetObjectSpace();

	virtual void Draw();
};