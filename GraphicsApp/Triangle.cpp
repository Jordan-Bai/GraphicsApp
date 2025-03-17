#include "Triangle.h"

Triangle::Triangle(Vertex vert1, Vertex vert2, Vertex vert3)
{
	m_verts.push_back(vert1);
	m_verts.push_back(vert2);
	m_verts.push_back(vert3); 

	InitObject();
}
