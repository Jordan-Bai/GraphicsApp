#pragma once

#include "Mesh.h"

class Quad : public Mesh
{
public:
	Quad(glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4);

	virtual void Draw() override;
};