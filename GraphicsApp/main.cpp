#define GLFW_INCLUDE_NONE
#include "glad.h"
#include "glfw3.h"

//#include <glm.hpp>

#include "Colour.h"
#include "FileLoading.h"
#include <iostream>

int main()
{
	// WINDOW SETUP
	//==========================================================================
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "GLFW COULD NOT INIT" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(1280, 720, "Test Window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		std::cout << "WINDOW COULD NOT BE CREATED" << std::endl;
		return -1;
	}

	// Make window the current window
	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		glfwTerminate();
		std::cout << "GLAD COULD NOT LOAD" << std::endl;
		return -1;
	}
	//==========================================================================
	
	const int noOfColours = 3;
	Colour colours[noOfColours] = { {1, 0, 0, 1}, {0, 0.8f, 0, 1}, {0, 0, 1, 1} };
	Colour currentColour = colours[0];
	Colour nextColour = colours[1];
	int colourIndex = 0;
	float lerp = 0;

	std::string output = LoadFileAsString("Test.txt");
	std::cout << output << std::endl;

	float lastFrameTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		// Clears the screen
		glClear(GL_COLOR_BUFFER_BIT);

		float timeBuffer = (float)glfwGetTime();
		float delta = timeBuffer - lastFrameTime;
		lastFrameTime = timeBuffer;

		lerp += delta;
		if (lerp > 0.99f)
		{
			lerp = 0;
			colourIndex++;
			if (colourIndex >= noOfColours)
			{
				colourIndex = 0;
			}

			currentColour = colours[colourIndex];
			if (colourIndex == noOfColours - 1)
			{
				nextColour = colours[0];
			}
			else
			{
				nextColour = colours[colourIndex + 1];
			}
		}

		Colour lerpedColour = LerpColours(currentColour, nextColour, lerp);

		// Set background colour
		glClearColor(lerpedColour.r, lerpedColour.g, lerpedColour.b, lerpedColour.a);


		// END OF FRAME
		glfwSwapBuffers(window); // Displays buffer we just wrote to 
		glfwPollEvents(); // Check for inputs
	}

	glfwTerminate();
	return 0;
}