#include "Object.h"

void Object::InitObject()
{
	glGenBuffers(1, &m_objectID);
	glBindBuffer(GL_ARRAY_BUFFER, m_objectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
