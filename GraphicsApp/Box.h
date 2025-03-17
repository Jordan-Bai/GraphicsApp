#pragma once

#include "Mesh.h"

class Box : public Mesh
{
public:
	Box(glm::vec3 center, glm::vec3 size);

	virtual void Draw() override;
};