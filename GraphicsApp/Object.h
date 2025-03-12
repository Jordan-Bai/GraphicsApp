#pragma once
#include <vector>
#include "GraphicsHeaders.h"
#include "glm.hpp"
#include "Utilities.h"

class Object
{
protected:
	GLuint m_objectID;
	glm::vec3 m_pos;
	std::vector<Vertex> m_verts; // Vertices stored in local space

public:
	Object()
		: m_pos(), m_objectID(0) // OBJECT ID 0 FOR TESTING
	{

	}

	void InitObject();
	virtual void Draw() = 0;
};