#pragma once
#include "Mesh.h"
#include "GameObject.h"

class ObjectType
{
public:
	float rad;
	float exclusionRad;
	int spawnAttempts;
	Mesh* mesh;
	Material* mat;

	float maxOverlap;
};

std::vector<GameObject*> PopulateMap(ObjectType object, Texture& heightMap);

glm::vec2 HeightRange(glm::vec2 pos, float radius, Texture& heightMap);

bool CanSpawn(glm::vec3 pos, ObjectType object, std::vector<glm::vec3>& posList, Texture& heightMap);