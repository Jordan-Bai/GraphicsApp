#include "GraphicsHeaders.h"

#include "Colour.h"
#include "Utilities.h"
#include "ShaderProgram.h"
#include "ext/matrix_transform.hpp"
#include "ext/matrix_clip_space.hpp"

#include "Triangle.h"
#include "Quad.h"
#include "Box.h"

#include <iostream>
#include <vector>

int main()
{
	// WINDOW SETUP
	//==========================================================================
	GLFWwindow* window;
	int width = 1280;
	int height = 720;

	if (!glfwInit())
	{
		std::cout << "GLFW COULD NOT INIT" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(width, height, "Test Window", nullptr, nullptr);
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
	Colour colours[noOfColours] = { {1, 0.3f, 0.3f, 1}, {0.3f, 0.7f, 0.3f, 1}, {0.3f, 0.3f, 1, 1} };
	Colour currentColour = colours[0];
	Colour nextColour = colours[1];
	int colourIndex = 0;
	float lerp = 0;


	glEnableVertexAttribArray(0); // layout location 0 in the vertex shader
	glEnableVertexAttribArray(1); // layout location 1 in the vertex shader
	ShaderProgram testShader("vertexShader1", "fragmentShader1");
	testShader.Use();
	testShader.SetFloatUniform("aspectRatio", (float)width / (float)height);

	glEnable(GL_DEPTH_TEST); // Enables depth buffer


	std::vector<Object*> gameObjects;
	Triangle* t = new Triangle(
		Vertex({ 0.5f, 0.5f, 0 }, { 0, 0, 0 }),
		Vertex({ 1, 0.5f, 0 }, { 0, 0, 0 }),
		Vertex({ 0.5f, 1, 1 }, { 0, 0, 0 }));
	gameObjects.push_back(t);
	//Quad* q = new Quad({ -0.5f, 0.5f, 0 }, { 0.5f, 0.5f, 0 }, { -0.5f, -0.5f, 0 }, { 0.5f, -0.5f, 0 });
	//gameObjects.push_back(q);
	//Box* b = new Box({ 0, 0, 0 }, { 0.5f, 0.5f, 0.5f });
	//gameObjects.push_back(b);

	float lastFrameTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		// Clears the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float timeBuffer = (float)glfwGetTime();
		float delta = timeBuffer - lastFrameTime;
		lastFrameTime = timeBuffer;

		lerp += delta * 0.2;
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

		// OBJECT STUFF
		//==========================================================================
		glm::mat4 objectSpace = glm::rotate(glm::mat4(1), (float)glfwGetTime(), glm::vec3());
		glm::mat4 cameraSpace = glm::lookAt(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(
			3.14159f / 2.0f, 
			(float)width / (float)height,	// Aspect ration
			0.3f,							// Near plane
			0.5f);							// Far plane

		glm::mat4 mvpMat = projection * cameraSpace * objectSpace; 
		// ^ Actually applied right to left, because of the way they're being multiplied 
		// (either column order or row order, not sure)
		
		testShader.SetMatrix4Uniform("mvpMat", mvpMat);

		for (Object* o : gameObjects)
		{
			o->Draw();
		}
		//==========================================================================

		// END OF FRAME
		glfwSwapBuffers(window); // Displays buffer we just wrote to 
		glfwPollEvents(); // Check for inputs
	}

	glfwTerminate();
	return 0;
}