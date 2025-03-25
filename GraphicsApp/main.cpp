#include "GraphicsHeaders.h"

#include "Colour.h"
#include "Utilities.h"
#include "ShaderProgram.h"
#include "ext/matrix_transform.hpp"
//#include "ext/matrix_clip_space.hpp"

#include "Application.h"
#include "Triangle.h"
#include "Quad.h"
#include "Box.h"
#include "Material.h"
#include "Camera.h"

#include <iostream>
#include <vector>

int main()
{
	// WINDOW SETUP
	//==========================================================================
	Application app;
	if (app.Initialize() == -1)
	{
		std::cout << "APPLICATION COULD NOT INITIALIZE" << std::endl;
		return -1;
	}
	//==========================================================================
	
	//const int noOfColours = 3;
	//Colour colours[noOfColours] = { {1, 0.3f, 0.3f, 1}, {0.3f, 0.7f, 0.3f, 1}, {0.3f, 0.3f, 1, 1} };
	//Colour currentColour = colours[0];
	//Colour nextColour = colours[1];
	//int colourIndex = 0;
	//float lerp = 0;

	glm::vec3 sunDirection = { 0, -1, -1 };

	glEnableVertexAttribArray(0); // layout location 0 in the vertex shader
	glEnableVertexAttribArray(1); // layout location 1 in the vertex shader
	glEnableVertexAttribArray(2); // layout location 2 in the vertex shader
	glEnableVertexAttribArray(3); // layout location 2 in the vertex shader
	ShaderProgram shader1("vertexShader1", "fragmentShader1");
	//shader1.Use();
	//shader1.SetVector3Uniform("sunDirection", glm::normalize(sunDirection));
	//shader1.SetFloatUniform("specPower", 2);

	ShaderProgram shader2("vertexShader2", "fragmentShader2");
	//shader2.Use();
	//shader2.SetVector3Uniform("sunDirection", glm::normalize(sunDirection));
	//shader2.SetFloatUniform("specPower", 2);

	Camera* cam = new Camera({ 0, 3.0f, 10.0f });
	//cam->Init(&app);
	app.AddObject(cam);
	app.SetCurrentCamera(cam);
	//cam.m_yRot = glm::radians(90.0f);

	glEnable(GL_DEPTH_TEST); // Enables depth buffer

	Mesh spearMesh;
	spearMesh.LoadFromFile("soulspear.obj");

	Texture albedo;
	albedo.LoadFileAsTexture("soulspear_diffuse.tga");
	Texture specular;
	specular.LoadFileAsTexture("soulspear_specular.tga");
	Texture normal;
	normal.LoadFileAsTexture("soulspear_normal.tga");

	Material mat1(&shader1, &albedo, &specular, &normal);
	Material mat2(&shader1, &normal, &normal, &normal);
	Material mat3(&shader2, &albedo, &specular, &normal);

	GameObject* spear1 = new GameObject();
	spear1->m_mesh = &spearMesh;
	spear1->m_mat = &mat1;
	app.AddObject(spear1);

	GameObject* spear2 = new GameObject();
	spear2->m_mesh = &spearMesh;
	spear2->m_mat = &mat2;
	spear2->m_pos = {3, 0, 0};
	app.AddObject(spear2);

	GameObject* spear3 = new GameObject();
	spear3->m_mesh = &spearMesh;
	spear3->m_mat = &mat3;
	spear3->m_pos = { -3, 0, 0 };
	app.AddObject(spear3);

	float lastFrameTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(app.GetWindow()))
	{
		float timeBuffer = (float)glfwGetTime();
		float delta = timeBuffer - lastFrameTime;
		lastFrameTime = timeBuffer;

		// CLEAR SCREEN
		//==========================================================================
		// Clears the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Set background colour
		glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		//==========================================================================
		app.Update(delta);
		//cam->Update(delta);
		if (app.GetKeyDown(GLFW_KEY_X))
		{
			shader1.ReloadShader();
			shader2.ReloadShader();
		}

		app.Draw(sunDirection, 2);

		// END OF FRAME
		//==========================================================================
		glfwSwapBuffers(app.GetWindow()); // Displays buffer we just wrote to 
		glfwPollEvents(); // Check for inputs
		//==========================================================================
	}

	glfwTerminate();
	return 0;
}