#include "Mesh.h"
#include "ext/matrix_transform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

Mesh::Mesh()
	: m_vertArray(), m_vertBuffer(), m_indexBuffer(), m_triCount(0)
{
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_vertBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
	glDeleteVertexArrays(1, &m_vertArray);
}


void Mesh::CreatePlaneMesh()
{
	std::vector<Vertex> verts;
	std::vector<int> indicies;

	// 0-----1
	// |     |
	// |     |
	// 2-----3
	verts.push_back({ { -1, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0 } });
	verts.push_back({ { 1, 1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 1 } });
	verts.push_back({ { -1, -1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 1, 0 } });
	verts.push_back({ { 1, -1, 0 }, { 0, 0, 1 }, { 1, 0, 0 }, { 1, 1 } });
	indicies.push_back(0);
	indicies.push_back(1);
	indicies.push_back(2);
	indicies.push_back(1);
	indicies.push_back(2);
	indicies.push_back(3);

	InitObject(verts, indicies);
}

void Mesh::CreateCubeMesh()
{
	std::vector<Vertex> verts;
	std::vector<int> indicies;

	std::vector<glm::vec2> offsets = { { -1, 1 }, { 1, 1 }, { -1, -1 }, { 1, -1 } };
	std::vector<glm::vec2> UVs = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } };
	//  (-1,1)    (1,1)
	//      0-----1
	//      |     |
	//      |     |
	//      2-----3
	// (-1,-1)    (1,-1)

	// Front face
	//   -------
	//  /|    /|
	// 0-----1 |
	// | /---|-/
	// |/    |/
	// 2-----3
	for (int i = 0; i < 4; i++)
	{
		verts.push_back({ { offsets[i].x, offsets[i].y, 1 }, { 0, 0, 1 }, { 1, 0, 0 }, UVs[i] });
	}

	// Back face
	//   0-----1
	//  /|    /|
	// ------- |
	// | 2---|-3
	// |/    |/
	// -------
	for (int i = 0; i < 4; i++)
	{
		verts.push_back({ { offsets[i].x, offsets[i].y, -1 }, { 0, 0, -1 }, { -1, 0, 0 }, UVs[i] });
	}

	// Right face
	//   ------3
	//  /|    /|
	// ------1 |
	// | /---|-2
	// |/    |/
	// ------0
	for (int i = 0; i < 4; i++)
	{
		verts.push_back({ { 1, offsets[i].x, offsets[i].y }, { 1, 0, 0 }, { 0, 0, 1 }, UVs[i] });
	}

	// Left face
	//   3------
	//  /|    /|
	// 1------ |
	// | 2---|-/
	// |/    |/
	// 0------
	for (int i = 0; i < 4; i++)
	{
		verts.push_back({ { -1, offsets[i].x, offsets[i].y }, { -1, 0, 0 }, { 0, 0, -1 }, UVs[i] });
	}

	// Top face
	//   2-----3
	//  /|    /|
	// 0-----1 |
	// | /---|-/
	// |/    |/
	// -------
	for (int i = 0; i < 4; i++)
	{
		verts.push_back({ { offsets[i].x, 1, offsets[i].y }, { 0, 1, 0 }, { 1, 0, 0 }, UVs[i] });
	}

	// Bottom face
	//   -------
	//  /|    /|
	// ------- |
	// | 2---|-3
	// |/    |/
	// 0-----1
	for (int i = 0; i < 4; i++)
	{
		verts.push_back({ { offsets[i].x, -1, offsets[i].y }, { 0, -1, 0 }, { -1, 0, 0 }, UVs[i] });
	}

	for (int i = 0; i < 24; i += 4)
	{
		indicies.push_back(i);
		indicies.push_back(i + 1);
		indicies.push_back(i + 2);
		indicies.push_back(i + 1);
		indicies.push_back(i + 2);
		indicies.push_back(i + 3);
	}

	InitObject(verts, indicies);
}

void Mesh::LoadFromFile(std::string fileName)
{
	Assimp::Importer importer;

	unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
	const aiScene* fileScene = importer.ReadFile(fileName, flags);
	// Second parameter is aiPostProcessSteps: determines which post process steps to apply
	// Triangulate makes sure every face is a triangle
	// ConvertToLeftHanded makes sure all UVs are the way round we want (better than FlipUVs)

	std::vector<Vertex> verts;
	std::vector<int> indicies;

	if (fileScene->mNumMeshes > 0)
	{
		aiMesh* mesh = fileScene->mMeshes[0]; // Just use the first mesh in the scene

		// Get the verts
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex newVertex;

			newVertex.pos.x = mesh->mVertices[i].x;
			newVertex.pos.y = mesh->mVertices[i].y;
			newVertex.pos.z = mesh->mVertices[i].z;

			newVertex.normal.x = mesh->mNormals[i].x;
			newVertex.normal.y = mesh->mNormals[i].y;
			newVertex.normal.z = mesh->mNormals[i].z;

			if (mesh->HasTangentsAndBitangents())
			{
				newVertex.tangent.x = mesh->mTangents[i].x;
				newVertex.tangent.y = mesh->mTangents[i].y;
				newVertex.tangent.z = mesh->mTangents[i].z;
			}

			if (mesh->HasTextureCoords(0)) // Check if they have texture coords
			{
				newVertex.UVcoord.x = mesh->mTextureCoords[0][i].x;
				newVertex.UVcoord.y = mesh->mTextureCoords[0][i].y;
			}

			verts.push_back(newVertex);
		}

		// Get the indices
		for (int i = 0; i < mesh->mNumFaces; i++) // For each face in the mesh
		{
			// For each vertex that makes up the face, add its index to the buffer
			for (int j = 0; j < 3; j++)
			{
				indicies.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		std::cout << "Verts: " << verts.size() << std::endl;
		std::cout << "Faces: " << mesh->mNumFaces << std::endl;

		// If the mesh doesn't have tangents & bitangents, output an error at the end (so we're no doing it for every face)
		if (!mesh->HasTangentsAndBitangents())
		{
			std::cout << "TANGENTS COULD NOT BE LOADED" << std::endl;
		}

	}

	InitObject(verts, indicies);
}

void Mesh::InitObject(std::vector<Vertex>& verts, std::vector<int>& indices)
{
	glGenVertexArrays(1, &m_vertArray);
	glGenBuffers(1, &m_vertBuffer);
	glGenBuffers(1, &m_indexBuffer);

	// Bind VAO & VBO (Order doesn't matter)
	//==========================================================================
	glBindVertexArray(m_vertArray);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verts.size(), verts.data(), GL_STATIC_DRAW);

	// Set vertex data
	//==========================================================================
	// Vertex data needs to be linked to the VAO *and* the VBO, so this must happen after both are bound
	glEnableVertexAttribArray(0); // layout location 0 in the vertex shader
	glEnableVertexAttribArray(1); // layout location 1 in the vertex shader
	glEnableVertexAttribArray(2); // layout location 2 in the vertex shader
	glEnableVertexAttribArray(3); // layout location 2 in the vertex shader

	// vertPos
	glVertexAttribPointer(0,	// Index: Corresponds to layout(location = _) in the vertex shader
		3,						// Size: number of the following type per vertex: in the case 3, since there are 3 floats per vertex
		GL_FLOAT,				// Type: type to read the data as
		GL_FALSE,				// Normalised: whether values should be maped to [-1, 1] (for signed) or [0, 1] (for unsigned)
		sizeof(Vertex),			// Stride: The amount to move through the vertex buffer per vertex
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

	// Bind IBO
	//==========================================================================
	// Needs to happen AFTER the VAO is bound, as otherwise they won't link up
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind VAO
	//==========================================================================
	// Don't need to unbind the VBO and IBO, as unbinding the VAO will automatically unbind them
	glBindVertexArray(0);

	m_triCount = indices.size() / 3;
}


void Mesh::Draw()
{
	// Bind VAO
	glBindVertexArray(m_vertArray);

	//glDrawArrays(GL_TRIANGLES, 0, m_verts.size()); // FOR DRAWING WITHout INDEX BUFFERS
	glDrawElements(GL_TRIANGLES, m_triCount * 3, GL_UNSIGNED_INT, 0); // FOR DRAWING WITH INDEX BUFFERS

	// Unbind VAO
	glBindVertexArray(0);
}
