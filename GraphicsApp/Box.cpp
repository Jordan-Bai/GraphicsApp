#include "Box.h"

Box::Box(glm::vec3 center, glm::vec3 size)
{
	m_pos = center;
	glm::vec3 colour(0, 0, 0);

	// Front face
	//   -------
	//  /|    /|
	// 1-----2 |
	// | /---|-/
	// |/    |/
	// 3-----4
	m_verts.push_back(Vertex(glm::vec3(-size.x, size.y, size.z), 	{ 1, 0, 0 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, size.z),		{ 1, 0, 0 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, size.z), 	{ 1, 0, 0 }));

	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, size.z),		{ 1, 0, 0 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, size.z),	{ 1, 0, 0 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, -size.y, size.z),	{ 1, 0, 0 }));

	// Back face
	//   1-----2
	//  /|    /|
	// ------- |
	// | 3---|-4
	// |/    |/
	// -------
	m_verts.push_back(Vertex(glm::vec3(-size.x, size.y, -size.z),	{ 0, 1, 1 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, -size.z),	{ 0, 1, 1 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, -size.z),	{ 0, 1, 1 }));

	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, -size.z),	{ 0, 1, 1 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, -size.z),	{ 0, 1, 1 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, -size.y, -size.z),	{ 0, 1, 1 }));

	// Right face
	//   ------2
	//  /|    /|
	// ------1 |
	// | /---|-4
	// |/    |/
	// ------3
	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, size.z),		{ 0, 1, 0 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, -size.z),	{ 0, 1, 0 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, -size.y, size.z),	{ 0, 1, 0 }));

	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, -size.z),	{ 0, 1, 0 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, -size.y, size.z),	{ 0, 1, 0 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, -size.y, -size.z),	{ 0, 1, 0 }));

	// Left face
	//   2------
	//  /|    /|
	// 1------ |
	// | 4---|-/
	// |/    |/
	// 3------
	m_verts.push_back(Vertex(glm::vec3(-size.x, size.y, size.z),	{ 1, 0, 1 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, size.y, -size.z),	{ 1, 0, 1 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, size.z),	{ 1, 0, 1 }));

	m_verts.push_back(Vertex(glm::vec3(-size.x, size.y, -size.z),	{ 1, 0, 1 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, size.z),	{ 1, 0, 1 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, -size.z),	{ 1, 0, 1 }));

	// Top face
	//   3-----4
	//  /|    /|
	// 1-----2 |
	// | /---|-/
	// |/    |/
	// -------
	m_verts.push_back(Vertex(glm::vec3(-size.x, size.y, size.z),	{ 0, 0, 1 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, size.z),		{ 0, 0, 1 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, size.y, -size.z),	{ 0, 0, 1 }));

	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, size.z),		{ 0, 0, 1 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, size.y, -size.z),	{ 0, 0, 1 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, size.y, -size.z),	{ 0, 0, 1 }));

	// Bottom face
	//   -------
	//  /|    /|
	// ------- |
	// | 3---|-4
	// |/    |/
	// 1-----2
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, size.z),	{ 1, 1, 0 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, -size.y, size.z),	{ 1, 1, 0 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, -size.z),	{ 1, 1, 0 }));

	m_verts.push_back(Vertex(glm::vec3(size.x, -size.y, size.z),	{ 1, 1, 0 }));
	m_verts.push_back(Vertex(glm::vec3(-size.x, -size.y, -size.z),	{ 1, 1, 0 }));
	m_verts.push_back(Vertex(glm::vec3(size.x, -size.y, -size.z),	{ 1, 1, 0 }));

	InitObject();
}

void Box::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffer);
	// vertPos
	glVertexAttribPointer(0,	// Index: Corresponds to layout(location = _) in the vertex shader
		3,						// Size: number of the following type per vertex: in the case 3, since there are 3 floats per vertex
		GL_FLOAT,				// Type: type to read the data as
		GL_FALSE,				// Normalised: whether values should be maped to [-1, 1] (for signed) or [0, 1] (for unsigned)
		sizeof(Vertex),			// Stride: ???
		0);						// Pointer: How far from the pointed to index the start of the data to read is
	// vertColour
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(const void*)(3 * sizeof(float)));
	// vertNormal
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(const void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// Draw front face
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//// Draw back face
	//glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	//// Draw right face
	//glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	//// Draw left face
	//glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	//// Draw top face
	//glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	//// Draw bottom face
	//glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);

	glDrawArrays(GL_TRIANGLES, 0, m_verts.size());
}
