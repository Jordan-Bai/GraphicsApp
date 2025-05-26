#include "RandomWalk.h"
#include <random>

Texture GenerateWalk(int gridSize, int steps)
{
    return GenerateWalk(gridSize, steps, time(0));
}

Texture GenerateWalk(int gridSize, int steps, int seed)
{
    std::mt19937 twister(seed);
    std::uniform_int_distribution<int> randomMove(-1, 1);

    std::vector<glm::vec3> heightMap(gridSize * gridSize);

    int x = gridSize / 2;
    int y = gridSize / 2;

    for (int i = 0; i < steps; i++)
    {
        //x += (rand() % 3) - 1; // -1, 0, or 1
        //y += (rand() % 3) - 1; // -1, 0, or 1
        x += randomMove(twister); // -1, 0, or 1
        y += randomMove(twister); // -1, 0, or 1

        x = x % gridSize;
        y = y % gridSize;
        if (x < 0)
        {
            x += gridSize;
        }
        if (y < 0)
        {
            y += gridSize;
        }

        heightMap[x + (y * gridSize)] += 0.1f;
    }

    return Texture(heightMap, { gridSize, gridSize });
}
