#include "PoissonDisk.h"
#include <vector>

std::vector<GameObject*> PopulateMap(ObjectType object, Texture& heightMap)
{
	std::vector<glm::vec3> mapData(heightMap.m_size.x * heightMap.m_size.y);
	glBindTexture(GL_TEXTURE_2D, heightMap.m_texture);
	glGetTexImage(GL_TEXTURE_2D,		// The type of texture to generate
		0,								// The 'mipmap level' (0 being the highest one)
		GL_RGB,							// Internal format (what channel format is used internally)
		GL_FLOAT,						// The type of the data
		mapData.data());

	std::vector<glm::vec3> posList; // Vec2 since they'll always be placed at the heighest y level

	float x = (float)rand() * heightMap.m_size.x / (float)RAND_MAX;
	float z = (float)rand() * heightMap.m_size.y / (float)RAND_MAX;
	int xInt = x;
	int zInt = z;
	float y = mapData[xInt + (zInt * heightMap.m_size.x)].x;
	y = HeightRange(glm::vec2(x, z), object.rad, heightMap).x;
	y = Remap(y, 0, 1, -1, 2);
	glm::vec3 firstPos(x, y, z);

	posList.push_back(firstPos);

	for (int i = 0; i < posList.size(); i++)
	{
		glm::vec3 pos = posList[i];

		// Add to open list
		for (int i = 0; i < object.spawnAttempts; i++)
		{
			float angle = (float)rand() * 3.14159 * 2 / (float)RAND_MAX;
			glm::vec3 nextPos = pos + (glm::vec3(cos(angle), 0, sin(angle)) * object.exclusionRad);

			if (nextPos.x < 0 || nextPos.x > heightMap.m_size.x - 1 || nextPos.z < 0 || nextPos.z > heightMap.m_size.y - 1)
			{
				continue;
			}
			xInt = nextPos.x;
			zInt = nextPos.z;
			//y = mapData[xInt + (zInt * heightMap.m_size.x)].x;
			y = HeightRange(glm::vec2(nextPos.x, nextPos.z), object.rad, heightMap).x;
			y = Remap(y, 0, 1, -1, 2);
			nextPos.y = y;

			if (CanSpawn(nextPos, object, posList, heightMap))
			{
				posList.push_back(nextPos);
			}
		}
	}

	std::vector<GameObject*> objects;
	for (glm::vec3 pos : posList)
	{
		GameObject* obj = new GameObject(object.mesh, object.mat);
		obj->m_pos = pos + glm::vec3(0, 1, 0);
		//glm::vec2 heightRange = HeightRange(glm::vec2(pos.x, pos.z), object.rad, heightMap);
		//obj->m_scale = { 1, heightRange.y - heightRange.x, 1 };
		objects.push_back(obj);
	}

	return objects;
}

glm::vec2 HeightRange(glm::vec2 pos, float radius, Texture& heightMap)
{
	std::vector<glm::vec3> mapData(heightMap.m_size.x * heightMap.m_size.y);
	glBindTexture(GL_TEXTURE_2D, heightMap.m_texture);
	glGetTexImage(GL_TEXTURE_2D,		// The type of texture to generate
		0,								// The 'mipmap level' (0 being the highest one)
		GL_RGB,							// Internal format (what channel format is used internally)
		GL_FLOAT,						// The type of the data
		mapData.data());

	int minX = Max(pos.x - radius, 0);
	int maxX = Min(pos.x + radius + 1, heightMap.m_size.x - 1);
	int minY = Max(pos.y - radius, 0);
	int maxY = Min(pos.y + radius + 1, heightMap.m_size.y - 1);
	//glm::vec2 posOffset = pos - glm::vec2(minX, minY);

	float minHeight = 1;
	float maxHeight = 0;
	
	for (int x = minX; x < maxX; x++)
	{
		for (int y = minY; y < maxY; y++)
		{
			float height = mapData[x + (y * heightMap.m_size.x)].x;

			glm::vec2 offset = pos - glm::vec2(x, y);
			if (glm::length(offset) <= radius)
			{
				if (height < minHeight)
				{
					minHeight = height;
				}
				if (height > maxHeight)
				{
					maxHeight = height;
				}
			}
			else
			{
				if (height < minHeight || height > maxHeight)
				{
					// Check the vertexes it forms a face with, and see if that face overlaps the circle

				}
			}
		}
	}

	return glm::vec2(minHeight, maxHeight);
}

bool CanSpawn(glm::vec3 pos, ObjectType object, std::vector<glm::vec3>& posList, Texture& heightMap)
{
	//if (pos.x < 0 || pos.x > heightMap.m_size.x || pos.z < 0 || pos.z > heightMap.m_size.y)
	//{
	//	return false;
	//}
	glm::vec2 heightRange = HeightRange(glm::vec2(pos.x, pos.z), object.rad, heightMap);
	float overlapHeight = heightRange.y - heightRange.x;
	overlapHeight = Remap(overlapHeight, 0, 1, -1, 2);
	if (overlapHeight > object.maxOverlap)
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
