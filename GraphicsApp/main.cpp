#include "Application.h"
#include "Camera.h"
#include "PointLight.h"

#include <imgui.h>

#include <iostream>

int main()
{
	// WINDOW SETUP
	//==========================================================================
	Application* app = Application::Get();
	if (app->Initialize() == -1)
	{
		std::cout << "ERROR(main): Application could not inititalize\n";
		return -1;
	}
	//==========================================================================


	// Initialise shaders
	//==========================================================================
	Shader vertShader1("shader1Vert", GL_VERTEX_SHADER);
	Shader fragShader1("shader1Frag", GL_FRAGMENT_SHADER);

	ShaderProgram shaderSunOnly(&vertShader1, &fragShader1);

	shaderSunOnly.m_uniforms.SetUniform("specPower", 10.0f);
	//==========================================================================


	// Initialise meshs/ textures/ materials
	//==========================================================================
	Mesh cubeMesh;
	cubeMesh.CreateCubeMesh();

	Texture blank(glm::vec3{ 0.7f, 0.7f, 0.7f });
	Texture blankNormal(glm::vec3{ 0.5f, 0.5f, 1.0f });

	Material defaultMat(&shaderSunOnly, &blank, &blank, &blankNormal);
	//==========================================================================


	// Create game objects
	//==========================================================================
	GameObject cube(&cubeMesh, &defaultMat);

	Camera cam({ 0, 3.0f, 10.0f });
	app->SetCurrentCamera(&cam);
	//==========================================================================
	

	// Final setup
	//==========================================================================
	glm::vec3 sunDirection = { 0, -1, -0.75 };
	glm::vec3 sunColour = { 1, 1, 1 };
	app->SetUniformInAllShaders("sunDirection", glm::normalize(sunDirection));
	app->SetUniformInAllShaders("sunColour", sunColour);
	app->ApplyAllUniforms();

	float lastFrameTime = (float)glfwGetTime();
	//==========================================================================

	while (!glfwWindowShouldClose(app->GetWindow()))
	{
		float timeBuffer = (float)glfwGetTime();
		float delta = timeBuffer - lastFrameTime;
		lastFrameTime = timeBuffer;

		// CLEAR SCREEN
		//==========================================================================
		// Clears the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Set background colour
		glClearColor(0.5f, 0.5f, 0.7f, 0.5f);
		//==========================================================================

		app->Update(delta);

		// BIND ALL UNIFORMS THAT CAN CHANGE
		//==========================================================================
		app->BindUniformInAllShaders("cameraPos", app->GetCurrentCamera()->GetPos());
		app->BindUniformInAllShaders("vpMat", app->GetVPMatrix());
		//==========================================================================

		// DO IMGUI STUFF
		//==========================================================================
		// Must be before app.Draw(), as the info needs to be stored using ImGui::Render() before its actually drawn
		ImGui::Begin("DEBUG MENU");

		ImGui::End();
		//==========================================================================

		app->Draw();

		// END OF FRAME
		//==========================================================================
		glfwSwapBuffers(app->GetWindow()); // Displays buffer we just wrote to 
		glfwPollEvents(); // Check for inputs
		//==========================================================================
	}

	glfwTerminate();
	return 0;
}