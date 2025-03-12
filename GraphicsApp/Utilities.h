#pragma once
#include <string>
#include "glm.hpp"

std::string LoadFileAsString(std::string fileName);

struct Vertex
{
	Vertex()
		:x(0), y(0), z(0), r(0), g(0), b(0)
	{
	}
	Vertex(glm::vec3 pos, glm::vec3 col)
		: x(pos.x), y(pos.y), z(pos.z), r(col.x), g(col.y), b(col.z)
	{
	}

	float x, y, z;
	float r, g, b;
};