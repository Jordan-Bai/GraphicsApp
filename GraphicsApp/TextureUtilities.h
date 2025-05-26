#pragma once
#include "Texture.h"

struct TextureData
{
	std::vector<glm::vec3> mapData;
	int sizeX;
	int sizeY;

	TextureData(Texture& tex);
	glm::vec3 GetNearest(int x, int y);
	glm::vec3 GetNearest(glm::vec2 pos);
	glm::vec3 GetLinear(glm::vec2 pos);
};

glm::vec2 HeightRangeOld(glm::vec2 pos, float radius, Texture& heightMap);
glm::vec2 HeightRange(glm::vec2 pos, float radius, TextureData& heightMap);

glm::vec3 GetRotation(glm::vec2 pos, float radius, TextureData& heightMap);