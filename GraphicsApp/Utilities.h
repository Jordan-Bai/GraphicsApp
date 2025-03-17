#pragma once
#include <string>
#include "glm.hpp"

std::string LoadFileAsString(std::string fileName);

struct Vertex
{
	Vertex()
		: pos(), colour(), normal()
	{
	}
	Vertex(glm::vec3 _pos, glm::vec3 _colour)
		: pos(_pos), colour(_colour), normal()
	{
	}
	Vertex(glm::vec3 _pos, glm::vec3 _colour, glm::vec3 _normal)
		: pos(_pos), colour(_colour), normal(_normal)
	{
	}

	glm::vec3 pos;
	glm::vec3 colour;
	glm::vec3 normal;
};