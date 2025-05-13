#include "PerlinNoise.h"

#include "Utilities.h"

Texture GeneratePerlinNoise(int gridSize, int tileRes)
{
	glm::vec2* perlinGrid = new glm::vec2[gridSize * gridSize];
	for (int i = 0; i < gridSize * gridSize; i++)
	{
		float x = ((float)rand() / (float)RAND_MAX);
		float y = ((float)rand() / (float)RAND_MAX);
		glm::vec2 randomVec((x * 2) - 1, (y * 2) - 1);
		randomVec /= glm::length(randomVec);
		perlinGrid[i] = randomVec;
	}

	int textureSize = (gridSize - 1) * tileRes;

	std::vector<glm::vec3> perlinNoise;
	for (int y = 0; y < textureSize; y++)
	{
		for (int x = 0; x < textureSize; x++)
		{
			int x1 = x / tileRes;
			int x2 = x1 + 1;
			int y1 = y / tileRes;
			int y2 = y1 + 1;

			//glm::vec2 pos((float)(x + 0.5f) / (float)tileRes, (float)(x + 0.5f) / (float)tileRes);
			glm::vec2 pos((float)(x + 0.5f) / (float)tileRes, (float)(y + 0.5f) / (float)tileRes);
			glm::vec2 offset1 = pos - glm::vec2(x1, y1);
			glm::vec2 offset2 = pos - glm::vec2(x1, y2);
			glm::vec2 offset3 = pos - glm::vec2(x2, y1);
			glm::vec2 offset4 = pos - glm::vec2(x2, y2);

			float dot1 = glm::dot(offset1, perlinGrid[x1 + (y1 * gridSize)]);
			float dot2 = glm::dot(offset2, perlinGrid[x1 + (y2 * gridSize)]);
			float dot3 = glm::dot(offset3, perlinGrid[x2 + (y1 * gridSize)]);
			float dot4 = glm::dot(offset4, perlinGrid[x2 + (y2 * gridSize)]);

			float lerpX = ((float)x / (float)tileRes) - x1;
			float lerpY = ((float)y / (float)tileRes) - y1;
			float leftLerp = Smoothstep(lerpY, dot1, dot2);
			float rightLerp = Smoothstep(lerpY, dot3, dot4);
			float total = Smoothstep(lerpX, leftLerp, rightLerp);
			total = Remap(total, -0.5, 0.5, 0, 1);
			//total = Remap(total, 0, 1, 0, 2);
			//total *= total;
			//total = Remap(total, 0, 4, 0, 1);
			perlinNoise.push_back(glm::vec3(total));
		}
	}


	return Texture(perlinNoise, glm::vec2(textureSize));
}
