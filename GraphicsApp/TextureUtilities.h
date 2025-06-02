#pragma once
#include "Texture.h"

struct TextureData
{
	std::vector<glm::vec3> mapData;
	int sizeX;
	int sizeY;

	TextureData(Texture& tex);
	glm::vec3 Get(int x, int y);

	glm::vec3 GetNearest(glm::vec2 pos);
	glm::vec3 GetLinear(glm::vec2 pos);

	glm::vec3 GetAverage(int x1, int y1, int x2, int y2);
	glm::vec3 GetAverage(glm::vec2 pos1, glm::vec2 pos2);
	glm::vec3 GetAverage(glm::vec2 pos, float radius);
};