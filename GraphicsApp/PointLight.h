#pragma once

#include "glm.hpp"

struct PointLight
{
	glm::vec3 pos;
	glm::vec3 col;
	float bright;

	PointLight()
		: pos(), col(), bright()
	{
	}
	PointLight(glm::vec3 position, glm::vec3 colour, float brightness)
		: pos(position), col(colour), bright(brightness)
	{
	}

	glm::vec3 GetColour()
	{
		return col * bright;
	}
};