#include "PoissonDisk.h"
#include <iostream>
//#include <random>

#include "gtc/quaternion.hpp"
//#include "gtx/quaternion.hpp"


std::vector<GameObject*> PopulateMap(ObjectType object, Texture& heightMap)
{
	return PopulateMap(object, heightMap, time(0));
}

std::vector<GameObject*> PopulateMap(ObjectType object, Texture& heightMap, int seed)
{
	TextureData mapData(heightMap);
	std::mt19937 twister(seed);
	std::uniform_real_distribution<float> randomX(0.0f, heightMap.m_size.x - 1);
	std::uniform_real_distribution<float> randomY(0.0f, heightMap.m_size.y - 1);
	std::uniform_real_distribution<float> randomAngle(0.0f, 3.14159 * 2);

	std::vector<glm::vec3> posList; // Vec2 since they'll always be placed at the heighest y level

	float x = randomX(twister);
	float z = randomY(twister);
	float y = object.GetBestHeight(glm::vec2(x, z), mapData);
	glm::vec3 firstPos(x, y, z);

	int i = 0;
	const int spawnAttempts = 1000;
	while (!CanSpawn(firstPos, object, posList, mapData))
	{
		if (i > spawnAttempts)
		{
			std::cout << "ERROR(PopulateMap): No valid spawn found within " << spawnAttempts << " spawns" << std::endl;
			return std::vector<GameObject*>();
		}
		x = randomX(twister);
		z = randomY(twister);
		y = object.GetBestHeight(glm::vec2(x, z), mapData);
		firstPos = {x, y, z};
		i++;
	}

	posList.push_back(firstPos);

	for (int i = 0; i < posList.size(); i++)
	{
		glm::vec3 pos = posList[i];

		// Add to open list
		for (int j = 0; j < object.spawnAttempts; j++)
		{
			float angle = randomAngle(twister);
			glm::vec3 nextPos = pos + (glm::vec3(cos(angle), 0, sin(angle)) * object.exclusionRad);

			if (nextPos.x < 0 || nextPos.x > heightMap.m_size.x - 1 || nextPos.z < 0 || nextPos.z > heightMap.m_size.y - 1)
			{
				continue;
			}
			y = object.GetBestHeight(glm::vec2(nextPos.x, nextPos.z), mapData);
			nextPos.y = y;

			if (CanSpawn(nextPos, object, posList, mapData))
			{
				posList.push_back(nextPos);
			}
		}
	}

	std::vector<GameObject*> objects;
	for (glm::vec3 pos : posList)
	{
		GameObject* obj = object.GenerateObject(pos, mapData, twister);

		objects.push_back(obj);
	}

	return objects;
}


bool Colliding(glm::vec3 pos, ObjectType object, std::vector<glm::vec3>& posList)
{
	for (glm::vec3 v : posList)
	{
		glm::vec3 offset = v - pos;
		if (glm::length(offset) < object.exclusionRad)
		{
			return true;
		}
	}
	return false;
}

bool CanSpawn(glm::vec3 pos, ObjectType object, std::vector<glm::vec3>& posList, TextureData& heightMap)
{
	//if (object.rotate)
	//{
	//	glm::vec3 rot = GetRotation(glm::vec2(pos.x, pos.z), object.rad, heightMap);
	//	if (abs(rot.x) > object.maxRotation || abs(rot.z) > object.maxRotation)
	//	{
	//		return false;
	//	}
	//}
	if (pos.x < 0 || pos.x > heightMap.sizeX - 1 || pos.z < 0 || pos.z > heightMap.sizeY - 1)
	{
		return false;
	}

	if (!object.CanSpawn(glm::vec2(pos.x, pos.z), heightMap))
	{
		return false;
	}

	for (glm::vec3 v : posList)
	{
		glm::vec3 offset = v - pos;
		if (glm::length(offset) < object.exclusionRad)
		{
			return false;
		}
	}

	return true;
}
