#pragma once

#include "Object.h"

class Box : public Object
{
public:
	Box(glm::vec3 center, glm::vec3 size);

	virtual void Draw() override;
};