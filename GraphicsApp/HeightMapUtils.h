#pragma once
#include "TextureUtilities.h"

//struct HeightMap
//{
//	TextureData tex;
//	glm::vec2 heightRange;
//
//	float Get(int x, int y);
//
//	float GetNearest(glm::vec2 pos);
//	float GetLinear(glm::vec2 pos);
//
//	float GetAverage(int x1, int y1, int x2, int y2);
//	float GetAverage(glm::vec2 pos1, glm::vec2 pos2);
//};

glm::vec2 HeightRange(glm::vec2 pos, float radius, TextureData& heightMap);
glm::vec2 HeightRange(glm::vec2 pos, float radius, TextureData& heightMap, glm::vec2 heightRange);

glm::vec3 GetRotation(glm::vec2 pos, float radius, TextureData& heightMap);
glm::vec3 GetRotation(glm::vec2 pos, float radius, TextureData& heightMap, glm::vec2 heightRange);