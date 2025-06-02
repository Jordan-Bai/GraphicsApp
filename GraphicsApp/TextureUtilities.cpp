#include "TextureUtilities.h"
#include "Utilities.h"
#include <iostream>

TextureData::TextureData(Texture& tex)
	:mapData(tex.m_size.x* tex.m_size.y), sizeX(tex.m_size.x), sizeY(tex.m_size.y)
{
	//std::vector<glm::vec3> mapData(heightMap.m_size.x * heightMap.m_size.y);
	glBindTexture(GL_TEXTURE_2D, tex.m_texture);
	glGetTexImage(GL_TEXTURE_2D,		// The type of texture to generate
		0,								// The 'mipmap level' (0 being the highest one)
		GL_RGB,							// Internal format (what channel format is used internally)
		GL_FLOAT,						// The type of the data
		mapData.data());
}

glm::vec3 TextureData::Get(int x, int y)
{
	if (x < 0 || x >= sizeX || y < 0 || y >= sizeY)
	{
		std::cout << "ERROR(TestureData::Get): Invalid position\n";
		return glm::vec3(-1);
	}
	//int testY = sizeY - y;
	//return mapData[x + (testY * sizeX)];
	return mapData[x + (y * sizeX)];
}


glm::vec3 TextureData::GetNearest(glm::vec2 pos)
{
	return Get(pos.x, pos.y);
}

glm::vec3 TextureData::GetLinear(glm::vec2 pos)
{
	if (pos.x < 0 || pos.x >= sizeX - 1 || pos.y < 0 || pos.y >= sizeY - 1)
	{
		std::cout << "ERROR(TextureData::GetLinear): Invalid position\n";
		return glm::vec3(-1);
	}
	int minX = pos.x;
	int minY = pos.y;
	//minY = sizeY - pos.y;
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


glm::vec3 TextureData::GetAverage(int x1, int y1, int x2, int y2)
{
	glm::vec3 colour(0);

	if (x1 < 0 || x2 >= sizeX || y1 < 0 || y2 >= sizeY || x1 > x2 || y1 > y2)
	{
		std::cout << "ERROR(TestureData::GetAverage): Invalid position \n";
		return glm::vec3(-1);
	}
	
	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
		{
			colour += Get(x, y);
		}
	}

	int tileCount = (x2 - x1 + 1) * (y2 - y1 + 1); // +1 for the x & y size since we're also including the max x & y pixels

	return colour / (float)tileCount;
}

glm::vec3 TextureData::GetAverage(glm::vec2 pos1, glm::vec2 pos2)
{
	glm::vec2 min(Min(pos1.x, pos2.x), Min(pos1.y, pos2.y));
	glm::vec2 max(Max(pos1.x, pos2.x), Max(pos1.y, pos2.y));
	return GetAverage(min.x, min.y, max.x, max.y);
}

glm::vec3 TextureData::GetAverage(glm::vec2 pos, float radius)
{
	float minX = Max(pos.x - radius, 0);
	float maxX = Min(pos.x + radius, sizeX - 1);
	float minY = Max(pos.y - radius, 0);
	float maxY = Min(pos.y + radius, sizeY - 1);

	int iterationsSqrt = 10;
	float stepSize = 2 * radius / (float)iterationsSqrt;

	glm::vec3 colour(0);
	int tileCount = 0;

	for (float x = minX; x < maxX; x += stepSize)
	{
		for (float y = minY; y < maxY; y += stepSize)
		{
			colour += GetLinear(glm::vec2(x, y));
			tileCount++;
		}
	}

	//int tileCount = iterationsSqrt * iterationsSqrt; // +1 for the x & y size since we're also including the max x & y pixels

	return colour / (float)tileCount;
}
