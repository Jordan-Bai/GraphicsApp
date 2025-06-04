#pragma once
#include "GameObject.h"
#include "TextureUtilities.h"
#include <random>

struct Variant
{
	Mesh* mesh;
	Material* mat;
	//glm::vec3 scale;

	Variant();
	Variant(Mesh* _mesh, Material* _mat);
	//Variant(Mesh* _mesh, Material* _mat, glm::vec3 _scale);
};

class ObjectType
{
public:
	float rad;
	float exclusionRad;
	int spawnAttempts;
	bool rotate;

	std::vector<Variant*> objectVariants;

	float minOverlap;
	float maxOverlap;
	float maxRotation;

	glm::vec3 scale;

	ObjectType();

	float GetBaseHeight(glm::vec2 pos, TextureData& heightMap);
	glm::vec3 GetHeightOffset(glm::vec3 pos, TextureData& heightMap);
	bool CanSpawn(glm::vec2 pos, TextureData& heightMap);
	//bool CanSpawn(glm::vec2 pos, TextureData& heightMap, glm::vec2 heightRange);

	GameObject* GenerateObject(glm::vec3 pos);
	GameObject* GenerateObject(glm::vec3 pos, TextureData& heightMap);
	GameObject* GenerateObject(glm::vec3 pos, TextureData& heightMap, std::mt19937& twister);
};