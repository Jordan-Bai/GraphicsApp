#pragma once

#include "Object.h"

class Triangle : public Object
{
public:
	Triangle(Vertex vert1, Vertex vert2, Vertex vert3);

	virtual void Draw() override;
};