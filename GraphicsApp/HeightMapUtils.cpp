#include "HeightMapUtils.h"
#include "Utilities.h"
//#include <iostream>

glm::vec2 HeightRange(glm::vec2 pos, float radius, TextureData& heightMap)
{
	float minX = Max(pos.x - radius, 0);
	float maxX = Min(pos.x + radius, heightMap.sizeX - 1);
	float minY = Max(pos.y - radius, 0);
	float maxY = Min(pos.y + radius, heightMap.sizeY - 1);

	int iterationsSqrt = 10;
	float stepSize = 2 * radius / (float)iterationsSqrt;

	float minHeight = 1;
	float maxHeight = 0;

	for (float x = minX; x < maxX; x += stepSize)
	{
		for (float y = minY; y < maxY; y += stepSize)
		{
			float height = heightMap.GetLinear(glm::vec2(x, y)).x;
			minHeight = Min(height, minHeight);
			maxHeight = Max(height, maxHeight);
		}
	}

	return glm::vec2(minHeight, maxHeight);
}

glm::vec2 HeightRange(glm::vec3 pos, float radius, glm::vec3 rot, TextureData& heightMap)
{
	float minX = Max(pos.x - radius, 0);
	float maxX = Min(pos.x + radius, heightMap.sizeX - 1);
	float minY = Max(pos.z - radius, 0);
	float maxY = Min(pos.z + radius, heightMap.sizeY - 1);

	int iterationsSqrt = 10;
	float stepSize = 2 * radius / (float)iterationsSqrt;

	float minHeight = 1;
	float maxHeight = -1;

	glm::vec3 xAxis(cos(rot.z), sin(rot.z), 0);
	glm::vec3 zAxis(0, sin(rot.x), cos(rot.x));
	glm::vec3 perp = glm::cross(xAxis, zAxis);
	//		     | C		     | Y
	//		A    |			X    |
	//		_____|			_____|
	//		    /			    /
	//		   / B			   / Z

	for (float x = minX; x < maxX; x += stepSize)
	{
		for (float y = minY; y < maxY; y += stepSize)
		{
			float height = heightMap.GetLinear(glm::vec2(x, y)).x;
			glm::vec3 offset = pos - glm::vec3(x, height, y);
			float rotatedHeight = glm::dot(perp, offset);
			minHeight = Min(rotatedHeight, minHeight);
			maxHeight = Max(rotatedHeight, maxHeight);
		}
	}

	return glm::vec2(minHeight, maxHeight);
}


glm::vec3 GetRotation(glm::vec2 pos, float radius, TextureData& heightMap)
{
	float minX = Max(pos.x - radius, 0);
	float maxX = Min(pos.x + radius, heightMap.sizeX - 1);
	float minY = Max(pos.y - radius, 0);
	float maxY = Min(pos.y + radius, heightMap.sizeY - 1);

	int iterationsSqrt = 10;
	float stepSize = 2 * radius / (float)iterationsSqrt;

	std::vector<glm::vec3> points3D;

	for (float x = minX; x < maxX; x += stepSize)
	{
		for (float y = minY; y < maxY; y += stepSize)
		{
			float height = heightMap.GetLinear(glm::vec2(x, y)).x;
			points3D.push_back(glm::vec3(x, height, -y));
			// ^ -y since it's meant to represent the z axis, where forward is negative
		}
	}

	glm::vec3 slope = BestFitLinear(points3D);

	return slope;
}