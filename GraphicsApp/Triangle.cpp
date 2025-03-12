#include "Triangle.h"

Triangle::Triangle(Vertex vert1, Vertex vert2, Vertex vert3)
{
	m_verts.push_back(vert1);
	m_verts.push_back(vert2);
	m_verts.push_back(vert3); 

	InitObject();
}

void Triangle::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_objectID);
	// vertPos
	glVertexAttribPointer(0,	// Index: Corresponds to layout(location = _) in the vertex shader
		3,						// Size: number of the following type per vertex: in the case 3, since there are 3 floats per vertex
		GL_FLOAT,				// Type: type to read the data as
		GL_FALSE,				// Normalised: whether values should be maped to [-1, 1] (for signed) or [0, 1] (for unsigned)
		6 * sizeof(float),		// Stride: ???
		0);						// Pointer: How far from the pointed to index the start of the data to read is
	// vertColour
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(float),
		(const void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, m_verts.size());
}
