#pragma once

#include "glm.hpp"

struct PointLight
{
	glm::vec3 pos;
	glm::vec3 col;

	PointLight()
		: pos(), col()
	{
	}
	PointLight(glm::vec3 position, glm::vec3 colour)
		: pos(position), col(colour)
	{
	}
};