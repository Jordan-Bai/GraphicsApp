#include "Box.h"

Box::Box(glm::vec3 center, glm::vec3 size)
{
	m_pos = center;

	//m_verts.push_back(center + glm::vec3(-size.x, size.y, -size.z));
	//m_verts.push_back(center + glm::vec3(size.x, size.y, -size.z));
	//m_verts.push_back(center + glm::vec3(-size.x, -size.y, -size.z));
	//m_verts.push_back(center + glm::vec3(size.x, -size.y, -size.z));
	//
	//m_verts.push_back(center + glm::vec3(-size.x / 2, size.y / 2, size.z));
	//m_verts.push_back(center + glm::vec3(size.x / 2, size.y / 2, size.z));
	//m_verts.push_back(center + glm::vec3(-size.x / 2, -size.y / 2, size.z));
	//m_verts.push_back(center + glm::vec3(size.x / 2, -size.y / 2, size.z));

	InitObject();
}

void Box::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_objectID);
	glVertexAttribPointer(0,	// Index: ???
		3,						// Size: number of the following type per vertex: in the case 3, since there are 3 floats per vertex
		GL_FLOAT,				// Type: type to read the data as
		GL_FALSE,				// Normalised: whether values should be maped to [-1, 1] (for signed) or [0, 1] (for unsigned)
		0,						// Stride: ???
		0);						// Pointer: How far from the pointed to index the start of the data to read is
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Draw back face
	//   0-----1
	//  /|    /|
	// ------- |
	// | 2---|-3
	// |/    |/
	// -------

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Draw front face
	//   -------
	//  /|    /|
	// 4-----5 |
	// | /---|-/
	// |/    |/
	// 6-----7
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
}
