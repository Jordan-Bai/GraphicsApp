#pragma once

#include "Object.h"

class Quad : public Object
{
public:
	Quad(glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4);

	virtual void Draw() override;
};