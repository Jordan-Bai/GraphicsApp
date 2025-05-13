#include "RandomWalk.h"

Texture GenerateWalk(int gridSize, int steps)
{
    std::vector<glm::vec3> heightMap(gridSize * gridSize);

    int x = gridSize / 2;
    int y = gridSize / 2;

    for (int i = 0; i < steps; i++)
    {
        //int testX = x;
        //int testY = y;

        x += (rand() % 3) - 1; // -1, 0, or 1
        y += (rand() % 3) - 1; // -1, 0, or 1

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

        //if (x + (y * gridSize) >= gridSize * gridSize || x + (y * gridSize) < 0)
        //{
        //    //heightMap[x + (y * gridSize)] += glm::vec3(0);
        //    heightMap[x + (y * gridSize)] += 0;
        //}
        //heightMap[x + (y * gridSize)] += glm::vec3(0.1);
        heightMap[x + (y * gridSize)] += 0.1f;
    }

    return Texture(heightMap, { gridSize, gridSize });
    //return Texture((char*)heightMap.data(), { gridSize, gridSize }, GL_RGB, GL_FLOAT);
}
