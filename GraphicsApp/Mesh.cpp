#include "Mesh.h"
#include "ext/matrix_transform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

Mesh::~Mesh()
{
	glDeleteBuffers(1, &m_vertBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
	glDeleteVertexArrays(1, &m_vertArray);
}

void Mesh::CreateCubeMesh()
{
	std::vector<Vertex> verts;
	std::vector<int> indicies;

	// Front face
	//   -------
	//  /|    /|
	// 0-----1 |
	// | /---|-/
	// |/    |/
	// 2-----3
	Vertex a0({ -1, 1, 1 },		{ 0, 0, 1 }, { 1, 0, 0 }, { 0, 0 });
	Vertex a1({ 1, 1, 1 },		{ 0, 0, 1 }, { 1, 0, 0 }, { 0, 1 });
	Vertex a2({ -1, -1, 1 },	{ 0, 0, 1 }, { 1, 0, 0 }, { 1, 0 });
	Vertex a3({ 1, -1, 1 },		{ 0, 0, 1 }, { 1, 0, 0 }, { 1, 1 });
	verts.push_back(a0);
	verts.push_back(a1);
	verts.push_back(a2);
	verts.push_back(a3);

	// Back face
	//   0-----1
	//  /|    /|
	// ------- |
	// | 2---|-3
	// |/    |/
	// -------
	Vertex b0({ -1, 1, -1 },	{ 0, 0, -1 }, { -1, 0, 0 }, { 0, 0 });
	Vertex b1({ 1, 1, -1 },		{ 0, 0, -1 }, { -1, 0, 0 }, { 0, 1 });
	Vertex b2({ -1, -1, -1 },	{ 0, 0, -1 }, { -1, 0, 0 }, { 1, 0 });
	Vertex b3({ 1, -1, -1 },	{ 0, 0, -1 }, { -1, 0, 0 }, { 1, 1 });
	verts.push_back(b0);
	verts.push_back(b1);
	verts.push_back(b2);
	verts.push_back(b3);

	// Left face
	//   1------
	//  /|    /|
	// 0------ |
	// | 3---|-/
	// |/    |/
	// 2------
	Vertex c0({ -1, 1, 1 },		{ -1, 0, 0 }, { 0, 0, -1 }, { 0, 0 });
	Vertex c1({ -1, 1, -1 },	{ -1, 0, 0 }, { 0, 0, -1 }, { 0, 1 });
	Vertex c2({ -1, -1, 1 },	{ -1, 0, 0 }, { 0, 0, -1 }, { 1, 0 });
	Vertex c3({ -1, -1, -1 },	{ -1, 0, 0 }, { 0, 0, -1 }, { 1, 1 });
	verts.push_back(c0);
	verts.push_back(c1);
	verts.push_back(c2);
	verts.push_back(c3);

	// Right face
	//   ------1
	//  /|    /|
	// ------0 |
	// | /---|-3
	// |/    |/
	// ------2
	Vertex d0({ 1, 1, 1 },		{ 1, 0, 0 }, { 0, 0, 1 }, { 0, 0 });
	Vertex d1({ 1, 1, -1 },		{ 1, 0, 0 }, { 0, 0, 1 }, { 0, 1 });
	Vertex d2({ 1, -1, 1 },		{ 1, 0, 0 }, { 0, 0, 1 }, { 1, 0 });
	Vertex d3({ 1, -1, -1 },	{ 1, 0, 0 }, { 0, 0, 1 }, { 1, 1 });
	verts.push_back(d0);
	verts.push_back(d1);
	verts.push_back(d2);
	verts.push_back(d3);

	// Top face
	//   2-----3
	//  /|    /|
	// 0-----1 |
	// | /---|-/
	// |/    |/
	// -------
	Vertex e0({ -1, 1, 1 },		{ 0, 1, 0 }, { 1, 0, 0 }, { 0, 0 });
	Vertex e1({ 1, 1, 1 },		{ 0, 1, 0 }, { 1, 0, 0 }, { 0, 1 });
	Vertex e2({ -1, 1, -1 },	{ 0, 1, 0 }, { 1, 0, 0 }, { 1, 0 });
	Vertex e3({ 1, 1, -1 },		{ 0, 1, 0 }, { 1, 0, 0 }, { 1, 1 });
	verts.push_back(e0);
	verts.push_back(e1);
	verts.push_back(e2);
	verts.push_back(e3);

	// Bottom face
	//   -------
	//  /|    /|
	// ------- |
	// | 2---|-3
	// |/    |/
	// 0-----1
	Vertex f0({ -1, -1, 1 },	{ 0, -1, 0 }, { 1, 0, 0 }, { 0, 0 });
	Vertex f1({ 1, -1, 1 },		{ 0, -1, 0 }, { 1, 0, 0 }, { 0, 1 });
	Vertex f2({ -1, -1, -1 },	{ 0, -1, 0 }, { 1, 0, 0 }, { 1, 0 });
	Vertex f3({ 1, -1, -1 },	{ 0, -1, 0 }, { 1, 0, 0 }, { 1, 1 });
	verts.push_back(f0);
	verts.push_back(f1);
	verts.push_back(f2);
	verts.push_back(f3);

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

void Mesh::CreateSmoothCubeMesh()
{
	std::vector<Vertex> verts;
	std::vector<int> indicies;

	//   4-----5
	//  /|    /|
	// 0-----1 |
	// | 6---|-7
	// |/    |/
	// 2-----3
	// Each normal pointing out from the center: means since normals are blended across a face, 
	// normals will act like a sphere
	Vertex c0({ -1, 1, 1 },		{ -0.5, 0.7071068, 0.5 },	{ -0.7071068, 0, 0.7071068 },	{ 0, 0 });
	Vertex c1({ 1, 1, 1 },		{ 0.5, 0.7071068, 0.5 },	{ -0.7071068, 0, -0.7071068 },	{ 0, 1 });
	Vertex c2({ -1, -1, 1 },	{ -0.5, -0.7071068, 0.5 },	{ -0.7071068, 0, 0.7071068 },	{ 1, 0 });
	Vertex c3({ 1, -1, 1 },		{ 0.5, -0.7071068, 0.5 },	{ -0.7071068, 0, -0.7071068 },	{ 1, 1 });
	Vertex c4({ -1, 1, -1 },	{ -0.5, 0.7071068, -0.5 },	{ 0.7071068, 0, 0.7071068 },	{ 0, 1 });
	Vertex c5({ 1, 1, -1 },		{ 0.5, 0.7071068, -0.5 },	{ 0.7071068, 0, -0.7071068 },	{ 0, 0 });
	Vertex c6({ -1, -1, -1 },	{ -0.5, -0.7071068, -0.5 },	{ 0.7071068, 0, 0.7071068 },	{ 1, 1 });
	Vertex c7({ 1, -1, -1 },	{ 0.5, -0.7071068, -0.5 },	{ 0.7071068, 0, -0.7071068 },	{ 1, 0 });

	verts.push_back(c0);
	verts.push_back(c1);
	verts.push_back(c2);
	verts.push_back(c3);
	verts.push_back(c4);
	verts.push_back(c5);
	verts.push_back(c6);
	verts.push_back(c7);

	// Front face
	indicies.push_back(0);
	indicies.push_back(1);
	indicies.push_back(2);
	indicies.push_back(1);
	indicies.push_back(2);
	indicies.push_back(3);

	// Right face
	indicies.push_back(1);
	indicies.push_back(5);
	indicies.push_back(3);
	indicies.push_back(5);
	indicies.push_back(3);
	indicies.push_back(7);

	// Bottom face
	indicies.push_back(7);
	indicies.push_back(6);
	indicies.push_back(3);
	indicies.push_back(6);
	indicies.push_back(3);
	indicies.push_back(2);

	// Left face
	indicies.push_back(4);
	indicies.push_back(0);
	indicies.push_back(6);
	indicies.push_back(0);
	indicies.push_back(6);
	indicies.push_back(2);

	// Top face
	indicies.push_back(4);
	indicies.push_back(5);
	indicies.push_back(0);
	indicies.push_back(5);
	indicies.push_back(0);
	indicies.push_back(1);

	// Back face
	indicies.push_back(4);
	indicies.push_back(5);
	indicies.push_back(6);
	indicies.push_back(5);
	indicies.push_back(6);
	indicies.push_back(7);

	InitObject(verts, indicies);
}

void Mesh::LoadFromFile(std::string fileName)
{
	Assimp::Importer importer;

	unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
	const aiScene* fileScene = importer.ReadFile(fileName, flags);
	// Second parameter is aiPostProcessSteps: determines which post process steps to apply
	// Triangulate makes sure every face is a triangle
	// JoinIdenticalVertices good for index buffering
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
				// COULD MESS STUFF UP IF UV'S ARE NOT IN 1ST UV CHANNEL
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

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	
	//// vertPos
	//glVertexAttribPointer(0,	// Index: Corresponds to layout(location = _) in the vertex shader
	//	3,						// Size: number of the following type per vertex: in the case 3, since there are 3 floats per vertex
	//	GL_FLOAT,				// Type: type to read the data as
	//	GL_FALSE,				// Normalised: whether values should be maped to [-1, 1] (for signed) or [0, 1] (for unsigned)
	//	sizeof(Vertex),			// Stride: The amount to move through the vertex buffer per vertex
	//	0);						// Pointer: How far from the pointed to index the start of the data to read is
	//// vertNormal
	//glVertexAttribPointer(1,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(Vertex),
	//	(const void*)(3 * sizeof(float)));
	//// vertTangent
	//glVertexAttribPointer(2,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(Vertex),
	//	(const void*)(6 * sizeof(float)));
	//// vertUVcoord
	//glVertexAttribPointer(3,
	//	2,						// Only 2 cuz it's a vector 2
	//	GL_FLOAT,
	//	GL_FALSE,
	//	sizeof(Vertex),
	//	(const void*)(9 * sizeof(float)));

	//glDrawArrays(GL_TRIANGLES, 0, m_verts.size()); // FOR DRAWING WITHout INDEX BUFFERS
	glDrawElements(GL_TRIANGLES, m_triCount * 3, GL_UNSIGNED_INT, 0); // FOR DRAWING WITH INDEX BUFFERS

	// Unbind VAO
	glBindVertexArray(0);
}
