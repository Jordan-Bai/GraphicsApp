#include "Quad.h"

Quad::Quad(glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 corner4)
{
	//m_verts.push_back(corner1);
	//m_verts.push_back(corner2);
	//m_verts.push_back(corner3);
	//m_verts.push_back(corner4);

	InitObject();
}

void Quad::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_objectID);
	glVertexAttribPointer(0,	// Index: ???
		3,						// Size: number of the following type per vertex: in the case 3, since there are 3 floats per vertex
		GL_FLOAT,				// Type: type to read the data as
		GL_FALSE,				// Normalised: whether values should be maped to [-1, 1] (for signed) or [0, 1] (for unsigned)
		0,						// Stride: ???
		0);						// Pointer: How far from the pointed to index the start of the data to read is
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_verts.size());
}
