#pragma once
#include "Texture.h"

Texture GeneratePerlinNoise(int gridSize, int tileRes);
Texture GeneratePerlinNoise(int gridSize, int tileRes, int seed);