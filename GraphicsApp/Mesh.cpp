#include "Mesh.h"
#include "ext/matrix_transform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_vertBuffer);
}

void Mesh::LoadFromFile(std::string fileName)
{
	Assimp::Importer importer;

	const aiScene* fileScene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);
	// Second parameter is aiPostProcessSteps: determines which post process steps to apply
	// Using triangulate here, which makes sure every face is a triangle
	// Others worth noting: aiProcess_JoinIdenticalVertices (don't need right now since we're just duplicating them anyway)

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

				newVertex.normal.x = mesh->mNormals[vertexIndex].x;
				newVertex.normal.y = mesh->mNormals[vertexIndex].y;
				newVertex.normal.z = mesh->mNormals[vertexIndex].z;

				if (mesh->HasTangentsAndBitangents())
				{
					newVertex.tangent.x = mesh->mTangents[vertexIndex].x;
					newVertex.tangent.y = mesh->mTangents[vertexIndex].y;
					newVertex.tangent.z = mesh->mTangents[vertexIndex].z;
				}

				if (mesh->HasTextureCoords(0)) // Check if they have texture coords
				{
					// COULD MESS STUFF UP IF UV'S ARE NOT IN 1ST UV CHANNEL
					newVertex.UVcoord.x = mesh->mTextureCoords[0][vertexIndex].x;
					newVertex.UVcoord.y = mesh->mTextureCoords[0][vertexIndex].y;
				}

				m_verts.push_back(newVertex);
			}
		}

		std::cout << "Verts: " << m_verts.size() << std::endl;
		std::cout << "Faces: " << mesh->mNumFaces << std::endl;

		// If the mesh doesn't have tangents & bitangents, calculate them all at the end
		if (!mesh->HasTangentsAndBitangents())
		{
			CalculateTangents();
		}

	}

	InitObject();
}

void Mesh::CalculateTangents()
{
	std::cout << "CALCULATING TANGENTS" << std::endl;

	// IF I DO INDEX BUFFER I'LL NEED TO REDO THIS
	for (int i = 0; i < m_verts.size(); i += 3)
	{

	}
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
	// vertNormal
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(const void*)(3 * sizeof(float)));
	// vertTangent
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(const void*)(6 * sizeof(float)));
	// vertUVcoord
	glVertexAttribPointer(3,
		2,						// Only 2 cuz it's a vector 2
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(const void*)(9 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_verts.size());
}
