#pragma once
#include <string>
#include "glm.hpp"

std::string LoadFileAsString(std::string fileName);

struct Vertex
{
	Vertex()
		: pos(), normal(), tangent(), UVcoord()
	{
	}
	Vertex(glm::vec3 _pos)
		: pos(_pos), normal(), tangent(), UVcoord()
	{
	}
	Vertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec3 _tangent)
		: pos(_pos), normal(_normal), tangent(_tangent), UVcoord()
	{
	}

	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 UVcoord;
};