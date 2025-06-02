#include "HeightMapUtils.h"
#include "Utilities.h"
//#include <iostream>

glm::vec2 HeightRange(glm::vec2 pos, float radius, TextureData& heightMap)
{
	return HeightRange(pos, radius, heightMap, glm::vec2(0, 1));
}

glm::vec2 HeightRange(glm::vec2 pos, float radius, TextureData& heightMap, glm::vec2 heightRange)
{
	//std::vector<glm::vec3> mapData(heightMap.m_size.x * heightMap.m_size.y);
	//glBindTexture(GL_TEXTURE_2D, heightMap.m_texture);
	//glGetTexImage(GL_TEXTURE_2D,		// The type of texture to generate
	//	0,								// The 'mipmap level' (0 being the highest one)
	//	GL_RGB,							// Internal format (what channel format is used internally)
	//	GL_FLOAT,						// The type of the data
	//	mapData.data());

	float minX = Max(pos.x - radius, 0);
	float maxX = Min(pos.x + radius, heightMap.sizeX - 1);
	float minY = Max(pos.y - radius, 0);
	float maxY = Min(pos.y + radius, heightMap.sizeY - 1);

	int iterationsSqrt = 10;
	float stepSize = 2 * radius / (float)iterationsSqrt;

	float minHeight = heightRange.y;
	float maxHeight = heightRange.x;

	for (float x = minX; x < maxX; x += stepSize)
	{
		for (float y = minY; y < maxY; y += stepSize)
		{
			//float height = GetColour(glm::vec2(x, y), mapData, heightMap.m_size.x, heightMap.m_size.y).x;
			float height = heightMap.GetLinear(glm::vec2(x, y)).x;
			height = Remap(height, 0, 1, heightRange.x, heightRange.y);
			minHeight = Min(height, minHeight);
			maxHeight = Max(height, maxHeight);
		}
	}

	return glm::vec2(minHeight, maxHeight);
}


glm::vec3 GetRotation(glm::vec2 pos, float radius, TextureData& heightMap)
{
	return GetRotation(pos, radius, heightMap, glm::vec2(0, 1));
}

glm::vec3 GetRotation(glm::vec2 pos, float radius, TextureData& heightMap, glm::vec2 heightRange)
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
			height = Remap(height, 0, 1, heightRange.x, heightRange.y);
			points3D.push_back(glm::vec3(x, height, -y));
			// ^ -y since it's meant to represent the z axis, where forward is negative
		}
	}

	glm::vec3 slope = BestFitLinear(points3D);

	return slope;
}