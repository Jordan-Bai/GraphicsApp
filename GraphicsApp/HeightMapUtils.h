#pragma once
#include "TextureUtilities.h"

glm::vec2 HeightRange(glm::vec2 pos, float radius, TextureData& heightMap);
glm::vec2 HeightRange(glm::vec3 pos, float radius, glm::vec3 rot, TextureData& heightMap);

glm::vec3 GetRotation(glm::vec2 pos, float radius, TextureData& heightMap);