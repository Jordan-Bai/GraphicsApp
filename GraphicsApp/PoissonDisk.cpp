#include "PoissonDisk.h"
#include <vector>
#include <iostream>

Variant::Variant()
	: mesh(nullptr), mat(nullptr), scale(1)
{
}

Variant::Variant(Mesh* _mesh, Material* _mat)
	: mesh(_mesh), mat(_mat), scale(1)
{
}

Variant::Variant(Mesh* _mesh, Material* _mat, glm::vec3 _scale)
	: mesh(_mesh), mat(_mat), scale(_scale)
{
}

ObjectType::ObjectType()
	: rad(1), exclusionRad(1), spawnAttempts(5), minOverlap(0), maxOverlap(1)
{
}


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
	float y = HeightRange(glm::vec2(x, z), object.rad, heightMap).x;
	y = Remap(y, 0, 1, -1, 2);
	glm::vec3 firstPos(x, y, z);
	while (!CanSpawn(firstPos, object, posList, heightMap))
	{
		x = (float)rand() * heightMap.m_size.x / (float)RAND_MAX;
		z = (float)rand() * heightMap.m_size.y / (float)RAND_MAX;
		y = HeightRange(glm::vec2(x, z), object.rad, heightMap).x;
		y = Remap(y, 0, 1, -1, 2);
		firstPos = {x, y, z};
	}

	posList.push_back(firstPos);

	for (int i = 0; i < posList.size(); i++)
	{
		glm::vec3 pos = posList[i];

		// Add to open list
		for (int j = 0; j < object.spawnAttempts; j++)
		{
			float angle = (float)rand() * 3.14159 * 2 / (float)RAND_MAX;
			glm::vec3 nextPos = pos + (glm::vec3(cos(angle), 0, sin(angle)) * object.exclusionRad);

			if (nextPos.x < 0 || nextPos.x > heightMap.m_size.x - 1 || nextPos.z < 0 || nextPos.z > heightMap.m_size.y - 1)
			{
				continue;
			}
			//y = mapData[xInt + (zInt * heightMap.m_size.x)].x;
			y = HeightRange(glm::vec2(nextPos.x, nextPos.z), object.rad, heightMap).x;
			y = Remap(y, 0, 1, -1, 2);
			nextPos.y = y - object.minOverlap;

			if (CanSpawn(nextPos, object, posList, heightMap))
			{
				posList.push_back(nextPos);
			}
		}
	}

	std::vector<GameObject*> objects;
	for (glm::vec3 pos : posList)
	{
		//Mesh* chosenMesh = nullptr;
		//if (object.meshes.size() == 0)
		//{
		//	std::cout << "ERROR(PopulateMap): Object type does not have mesh";
		//}
		//else
		//{
		//	int index = rand() % object.meshes.size();
		//	chosenMesh = object.meshes[index];
		//}
		//
		//GameObject* obj = new GameObject(chosenMesh, object.mat);
		//obj->m_pos = pos;// +glm::vec3(0, 1, 0);
		//obj->m_scale = object.scale;
		Variant variant;
		if (object.objectVariants.size() == 0)
		{
			std::cout << "ERROR(PopulateMap): Object type does not have variants";
		}
		else
		{
			int index = rand() % object.objectVariants.size();
			variant = object.objectVariants[index];
		}
		GameObject* obj = new GameObject(variant.mesh, variant.mat);
		obj->m_scale = variant.scale;
		obj->m_pos = pos;

		objects.push_back(obj);
	}

	return objects;
}

glm::vec2 HeightRangeOld(glm::vec2 pos, float radius, Texture& heightMap)
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
			float magSqrd = (offset.x * offset.x) + (offset.y * offset.y);
			if (magSqrd <= radius * radius)
			{
				//if (height < minHeight)
				//{
				//	minHeight = height;
				//}
				//if (height > maxHeight)
				//{
				//	maxHeight = height;
				//}
				minHeight = Min(height, minHeight);
				maxHeight = Max(height, maxHeight);
			}
			else
			{
				if (height < minHeight || height > maxHeight)
				{
					// Check the vertexes it forms a face with, and see if that face overlaps the circle
					//	   2--3
					//	  /| /|
					//	 / |/ |
					//	1--X--4
					//	| /| /
					//	|/ |/
					//	6--5
					std::vector<glm::vec2> connectedPoints;
					if (x > 0) // Point 1 exists
					{
						connectedPoints.push_back(glm::vec2(x - 1, y));
					}
					if (y > 0) // Point 2 exists
					{
						connectedPoints.push_back(glm::vec2(x, y - 1));

						if (x < heightMap.m_size.x) // Point 3 exists
						{
							connectedPoints.push_back(glm::vec2(x + 1, y - 1));
						}
					}
					if (x < heightMap.m_size.x) // Point 4 exists
					{
						connectedPoints.push_back(glm::vec2(x + 1, y));
					}
					if (y < heightMap.m_size.y) // Point 5 exists
					{
						connectedPoints.push_back(glm::vec2(x, y + 1));

						if (x < heightMap.m_size.x) // Point 6 exists
						{
							connectedPoints.push_back(glm::vec2(x - 1, y + 1));
						}
					}

					for (glm::vec2 p : connectedPoints)
					{
						offset = pos - p;
						magSqrd = (offset.x * offset.x) + (offset.y * offset.y);
						if (magSqrd <= radius * radius)
						{
							// Find the overlap point
							//float yOffset = pos.y - p.y;
							//float xOffset = pos.x - p.x;
							//xOffset = sqrt((radius * radius) - (yOffset * yOffset)) - xOffset;
							//glm::vec2 overlapPoint(p.y, p.x + xOffset);

							glm::vec2 dir = p - glm::vec2(x, y);
							dir = glm::normalize(dir);
							glm::vec2 perp(-dir.y, dir.x);
							//perp = glm::normalize(perp);
							float perpLength = glm::dot(perp, offset);
							float dirLength = sqrt((radius * radius) - (perpLength * perpLength));
							glm::vec2 overlapPoint = pos + (perp * perpLength) + (dir * dirLength);

							//height = GetColour(overlapPoint, mapData, heightMap.m_size.x, heightMap.m_size.y).x;
							//minHeight = Min(height, minHeight);
							//maxHeight = Max(height, maxHeight);
						}
					}
				}
			}
		}
	}

	return glm::vec2(minHeight, maxHeight);
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

	float minX = Max(pos.x - radius, 0);
	float maxX = Min(pos.x + radius, heightMap.m_size.x - 1);
	float minY = Max(pos.y - radius, 0);
	float maxY = Min(pos.y + radius, heightMap.m_size.y - 1);

	int iterationsSqrt = 10;
	float stepSize = 2 * radius / (float)iterationsSqrt;

	float minHeight = 1;
	float maxHeight = 0;

	for (float x = minX; x < maxX; x += stepSize)
	{
		for (float y = minY; y < maxY; y += stepSize)
		{
			float height = GetColour(glm::vec2(x, y), mapData, heightMap.m_size.x, heightMap.m_size.y).x;
			minHeight = Min(height, minHeight);
			maxHeight = Max(height, maxHeight);
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
	overlapHeight = Remap(overlapHeight, 0, 1, 0, 3);
	if (overlapHeight > object.maxOverlap - object.minOverlap)
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


glm::vec3 GetColour(glm::vec2 pos, Texture& heightMap)
{
	std::vector<glm::vec3> mapData(heightMap.m_size.x * heightMap.m_size.y);
	glBindTexture(GL_TEXTURE_2D, heightMap.m_texture);
	glGetTexImage(GL_TEXTURE_2D,		// The type of texture to generate
		0,								// The 'mipmap level' (0 being the highest one)
		GL_RGB,							// Internal format (what channel format is used internally)
		GL_FLOAT,						// The type of the data
		mapData.data());

	return GetColour(pos, mapData, heightMap.m_size.x, heightMap.m_size.y);
}

glm::vec3 GetColour(glm::vec2 pos, std::vector<glm::vec3>& mapData, int sizeX, int sizeY)
{
	if (pos.x < 0 || pos.x >= sizeX - 1 || pos.y < 0 || pos.y >= sizeY - 1)
	{
		std::cout << "ERROR(GetColour): Invalid position";
		return glm::vec3(-1);
	}
	int minX = pos.x;
	int minY = pos.y;
	float xLerp = pos.x - minX;
	float yLerp = pos.y - minY;
	glm::vec3 botLeftCol = mapData[minX + (minY * sizeX)];
	glm::vec3 botRightCol = mapData[minX + 1 + (minY * sizeX)];
	glm::vec3 topLeftCol = mapData[minX + ((minY + 1) * sizeX)];
	glm::vec3 topRightCol = mapData[minX + 1 + ((minY + 1) * sizeX)];
	glm::vec3 botColour = Lerp(xLerp, botLeftCol, botRightCol);
	glm::vec3 topColour = Lerp(xLerp, topLeftCol, topRightCol);

	return Lerp(yLerp, botColour, topColour);
}
