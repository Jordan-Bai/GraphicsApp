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

glm::vec3 TextureData::GetNearest(int x, int y)
{
	if (x < 0 || x >= sizeX || y < 0 || y >= sizeY)
	{
		std::cout << "ERROR(TestureData::GetNearest): Invalid position";
		return glm::vec3(-1);
	}
	return mapData[x + (y * sizeX)];
}

glm::vec3 TextureData::GetNearest(glm::vec2 pos)
{
	return GetNearest(pos.x, pos.y);
}

glm::vec3 TextureData::GetLinear(glm::vec2 pos)
{
	if (pos.x < 0 || pos.x >= sizeX - 1 || pos.y < 0 || pos.y >= sizeY - 1)
	{
		std::cout << "ERROR(TextureData::GetLinear): Invalid position";
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

glm::vec2 HeightRange(glm::vec2 pos, float radius, TextureData& heightMap)
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

	float minHeight = 1;
	float maxHeight = 0;

	for (float x = minX; x < maxX; x += stepSize)
	{
		for (float y = minY; y < maxY; y += stepSize)
		{
			//float height = GetColour(glm::vec2(x, y), mapData, heightMap.m_size.x, heightMap.m_size.y).x;
			float height = heightMap.GetLinear(glm::vec2(x, y)).x;
			minHeight = Min(height, minHeight);
			maxHeight = Max(height, maxHeight);
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

	//std::vector<glm::vec2> points;
	std::vector<glm::vec3> points3D;

	for (float x = minX; x < maxX; x += stepSize)
	{
		for (float y = minY; y < maxY; y += stepSize)
		{
			//float height = GetColour(glm::vec2(x, y), mapData, heightMap.m_size.x, heightMap.m_size.y).x;
			float height = heightMap.GetLinear(glm::vec2(x, y)).x;
			//points.push_back(glm::vec2(x, height));
			points3D.push_back(glm::vec3(x, height, -y));
			// ^ -y since it's meant to represent the z axis, where forward is negative
		}
	}

	//float slope = BestFitLinear(points);
	glm::vec3 s = BestFitLinear(points3D);

	//return glm::vec3(0, 0, slope);
	//return glm::vec3(s.y, 0, 0);
	return s;
}
