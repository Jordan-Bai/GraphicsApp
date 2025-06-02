#pragma once
#include "GameObject.h"
#include "ObjectType.h"

std::vector<GameObject*> PopulateMap(ObjectType object, Texture& heightMap);
std::vector<GameObject*> PopulateMap(ObjectType object, Texture& heightMap, int seed);
std::vector<GameObject*> PopulateMap(ObjectType object, Texture& heightMap, glm::vec2 heightRange);
std::vector<GameObject*> PopulateMap(ObjectType object, Texture& heightMap, glm::vec2 heightRange, int seed);

bool Colliding(glm::vec3 pos, ObjectType object, std::vector<glm::vec3>& posList);
bool CanSpawn(glm::vec3 pos, ObjectType object, std::vector<glm::vec3>& posList, TextureData& heightMap);
