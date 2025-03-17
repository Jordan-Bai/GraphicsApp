#include "Mesh.h"
#include "ext/matrix_transform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

void Mesh::LoadFromFile(std::string fileName)
{
	Assimp::Importer importer;

	const aiScene* fileScene = importer.ReadFile(fileName, aiProcess_Triangulate);
	// Second parameter is aiPostProcessSteps: determines which post process steps to apply
	// Using triangulate here, which makes sure every face is a triangle

	if (fileScene->mNumMeshes > 0)
	{
		aiMesh* mesh = fileScene->mMeshes[0]; // Just use the first mesh in the scene

		// For each face in the mesh:
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			// For each vertex that makes up the face, add it to the list of vertices
			// (can replace this once I add index buffering)
			for (int j = 0; j < 3; j++)
			{
				int vertexIndex = mesh->mFaces[i].mIndices[j];
				Vertex newVertex;

				newVertex.pos.x = mesh->mVertices[vertexIndex].x;
				newVertex.pos.y = mesh->mVertices[vertexIndex].y;
				newVertex.pos.z = mesh->mVertices[vertexIndex].z;

				newVertex.colour = { 0, 0, 0 };
				//newVertex.colour.x = mesh->mColors[vertexIndex]->r;
				//newVertex.colour.y = mesh->mColors[vertexIndex]->g;
				//newVertex.colour.z = mesh->mColors[vertexIndex]->b;

				newVertex.normal.x = mesh->mNormals[vertexIndex].x;
				newVertex.normal.y = mesh->mNormals[vertexIndex].y;
				newVertex.normal.z = mesh->mNormals[vertexIndex].z;

				m_verts.push_back(newVertex);
			}
		}

		std::cout << "Verts: " << m_verts.size() << std::endl;
		std::cout << "Faces: " << mesh->mNumFaces << std::endl;
	}

	InitObject();
}

void Mesh::InitObject()
{
	glGenBuffers(1, &m_vertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

glm::mat4 Mesh::GetObjectSpace()
{
	return glm::translate(glm::mat4(1), m_pos);
}

void Mesh::Draw()
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

	glDrawArrays(GL_TRIANGLES, 0, m_verts.size());
}
