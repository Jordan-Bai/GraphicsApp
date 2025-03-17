#pragma once

#include "Mesh.h"

class Triangle : public Mesh
{
public:
	Triangle(Vertex vert1, Vertex vert2, Vertex vert3);
};