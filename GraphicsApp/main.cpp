#include "GraphicsHeaders.h"

#include "Colour.h"
#include "Utilities.h"
#include "ShaderProgram.h"
#include "ext/matrix_transform.hpp"
#include "ext/matrix_clip_space.hpp"

#include "Triangle.h"
#include "Quad.h"
#include "Box.h"
#include "Texture.h"
#include "Camera.h"

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
	glEnableVertexAttribArray(2); // layout location 2 in the vertex shader
	glEnableVertexAttribArray(3); // layout location 2 in the vertex shader
	ShaderProgram testShader("vertexShader1", "fragmentShader1");
	testShader.Use();

	glm::vec3 sunDirection = { 0, -1, -1 };
	testShader.SetVector3Uniform("sunDirection", glm::normalize(sunDirection));

	Camera cam({ 0, 3.0f, 10.0f });
	//cam.m_yRot = glm::radians(90.0f);
	//glm::vec3 cameraPos(0, 3.0f, 5.0f);
	//glm::vec3 lookOffset(0, 0, -5.0f);
	float speed = 5;
	//testShader.SetVector3Uniform("cameraPos", cameraPos);

	testShader.SetFloatUniform("specPower", 16);

	glEnable(GL_DEPTH_TEST); // Enables depth buffer


	std::vector<Mesh*> gameObjects;
	//Triangle* t1 = new Triangle(
	//	Vertex({ 0, 0, 0 }, { 0, 0, 0 }),
	//	Vertex({ 1, 0, 0 }, { 0, 0, 0 }),
	//	Vertex({ 0, 1, 0 }, { 0, 0, 0 }));
	//gameObjects.push_back(t1);
	//Triangle* t2 = new Triangle(
	//	Vertex({ 1, 1,  0.5f }, { 1, 1, 1 }),
	//	Vertex({ -1, 0, 0.5f }, { 1, 1, 1 }),
	//	Vertex({ 0, -1, 0.5f }, { 1, 1, 1 }));
	//gameObjects.push_back(t2);
	//Triangle* t3 = new Triangle(
	//	Vertex({ 0, 0, 1 }, { 1, 0, 0 }),
	//	Vertex({ 1, 0, 1 }, { 1, 0, 0 }),
	//	Vertex({ 0, 1, 1 }, { 1, 0, 0 }));
	//gameObjects.push_back(t3);
	
	//Quad* q = new Quad({ -0.5f, 0.5f, 0 }, { 0.5f, 0.5f, 0 }, { -0.5f, -0.5f, 0 }, { 0.5f, -0.5f, 0 });
	//gameObjects.push_back(q);
	//Box* b1 = new Box({ 0, 0, 0 }, { 0.5f, 0.5f, 0.5f });
	//gameObjects.push_back(b1);
	//Box* b2 = new Box({ 2, 0, 0 }, { 0.5f, 0.5f, 0.5f });
	//gameObjects.push_back(b2);

	Mesh* spear = new Mesh();
	spear->LoadFromFile("soulspear.obj");
	gameObjects.push_back(spear);

	Texture albedo;
	albedo.LoadFileAsTexture("soulspear_diffuse.tga");
	Texture specular;
	specular.LoadFileAsTexture("soulspear_specular.tga");

	float lastFrameTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		// Clears the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float timeBuffer = (float)glfwGetTime();
		float delta = timeBuffer - lastFrameTime;
		lastFrameTime = timeBuffer;

		// CHANGE BACKGROUND COLOUR
		//==========================================================================
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
		//==========================================================================

		// CONTROLS
		//==========================================================================
		cam.Update(window, delta);
		testShader.SetVector3Uniform("cameraPos", cam.GetPos());
		//==========================================================================


		// OBJECT STUFF
		//==========================================================================
		glm::mat4 rotation = glm::rotate(glm::mat4(1), (float)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		// ^ For testing
		glm::mat4 view = cam.GetViewMatrix(); //glm::lookAt(cameraPos, cameraPos + lookOffset, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(
			3.14159f / 4.0f, 
			(float)width / (float)height,		// Aspect ratio
			0.3f,								// Near plane
			1000.0f);							// Far plane

		glm::mat4 vpMat = projection * view;
		// ^ Actually applied right to left, because of the way they're being multiplied 
		// (openGL uses column-major order for matricies)
		testShader.SetMatrix4Uniform("vpMat", vpMat);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo.m_texture);
		//testShader.SetIntUniform("albedoMap", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular.m_texture);
		testShader.SetIntUniform("albedoMap", 0);
		testShader.SetIntUniform("specularMap", 1);

		for (Mesh* m : gameObjects)
		{
			glm::mat4 modelSpace = rotation * m->GetObjectSpace();

			testShader.SetMatrix4Uniform("modelMat", modelSpace);

			m->Draw();
		}
		//==========================================================================

		// END OF FRAME
		glfwSwapBuffers(window); // Displays buffer we just wrote to 
		glfwPollEvents(); // Check for inputs
	}

	glfwTerminate();
	return 0;
}