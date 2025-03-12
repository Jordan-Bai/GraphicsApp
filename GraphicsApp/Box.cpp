#include "Box.h"

Box::Box(glm::vec3 center, glm::vec3 size)
{
	m_pos = center;
	glm::vec3 colour(0, 0, 0);

	m_verts.push_back(Vertex(glm::vec3(-size.x, size.y, size.z), colour));
	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, size.z), colour));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, size.z), colour));
	m_verts.push_back(Vertex(glm::vec3(size.x, -size.y, size.z), colour));

	m_verts.push_back(Vertex(glm::vec3(-size.x, size.y, -size.z), colour));
	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, -size.z), colour));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, -size.z), colour));
	m_verts.push_back(Vertex(glm::vec3(size.x, -size.y, -size.z), colour));

	InitObject();
}

void Box::Draw()
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

	// Draw front face
	//   -------
	//  /|    /|
	// 0-----1 |
	// | /---|-/
	// |/    |/
	// 2-----3

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Draw back face
	//   4-----5
	//  /|    /|
	// ------- |
	// | 6---|-7
	// |/    |/
	// -------

	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
}
