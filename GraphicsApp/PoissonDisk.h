#pragma once
#include "Mesh.h"
#include "GameObject.h"

struct Variant
{
	Mesh* mesh;
	Material* mat;
	glm::vec3 scale;

	Variant();
	Variant(Mesh* _mesh, Material* _mat);
	Variant(Mesh* _mesh, Material* _mat, glm::vec3 _scale);
};

class ObjectType
{
public:
	float rad;
	float exclusionRad;
	int spawnAttempts;

	std::vector<Variant> objectVariants;
	//std::vector<Mesh*> meshes;
	//Material* mat;
	//glm::vec3 scale;

	float minOverlap;
	float maxOverlap;

	ObjectType();
};

std::vector<GameObject*> PopulateMap(ObjectType object, Texture& heightMap);

glm::vec2 HeightRangeOld(glm::vec2 pos, float radius, Texture& heightMap);
glm::vec2 HeightRange(glm::vec2 pos, float radius, Texture& heightMap);

bool CanSpawn(glm::vec3 pos, ObjectType object, std::vector<glm::vec3>& posList, Texture& heightMap);

glm::vec3 GetColour(glm::vec2 pos, Texture& heightMap);
glm::vec3 GetColour(glm::vec2 pos, std::vector<glm::vec3>& mapData, int sizeX, int sizeY);