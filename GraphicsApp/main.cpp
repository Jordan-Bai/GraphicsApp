#include "Application.h"
#include "Camera.h"
#include "PointLight.h"
#include "PerlinNoise.h"
#include "RandomWalk.h"

#include <ext.hpp>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

int main()
{
	// WINDOW SETUP
	//==========================================================================
	Application* app = Application::Get();
	if (app->Initialize() == -1)
	{
		std::cout << "APPLICATION COULD NOT INITIALIZE" << std::endl;
		return -1;
	}
	//==========================================================================

	glm::vec3 sunDirection = { 0, -1, -0.75 };
	glm::vec3 sunColour = { 1, 1, 1 };

	// Initialise shaders
	//==========================================================================
	Shader vertShader1("shader1Vert", GL_VERTEX_SHADER);
	Shader fragShader1("shader1Frag", GL_FRAGMENT_SHADER);
	Shader fragShader2("shader2Frag", GL_FRAGMENT_SHADER);
	Shader fragShaderUnlit("shaderUnlitFrag", GL_FRAGMENT_SHADER);
	Shader vertShaderScreenspace("shaderScreenspaceVert", GL_VERTEX_SHADER);
	Shader fragShaderScreenspace("shaderScreenspaceFrag", GL_FRAGMENT_SHADER);

	ShaderProgram shaderSunOnly(&vertShader1, &fragShader1);
	ShaderProgram shaderAllLights(&vertShader1, &fragShader2);
	ShaderProgram shaderUnlit(&vertShader1, &fragShaderUnlit);
	ShaderProgram shaderScreenspace(&vertShaderScreenspace, &fragShaderScreenspace);

	shaderSunOnly.m_uniforms.SetUniform("specPower", 10.0f);
	shaderAllLights.m_uniforms.SetUniform("specPower", 10.0f);
	shaderScreenspace.m_uniforms.SetUniform("aspectRatio", app->GetAspectRatio());
	//==========================================================================

	srand(time(0));
	//srand(1);
	const int gridSize = 8;
	const int tileRes = 8;
	Texture perlinTex = GeneratePerlinNoise(gridSize, tileRes);

	const int walkGridSize = 100;
	Texture randomWalkTex = GenerateWalk(walkGridSize, 20000);
	glTextureParameteri(randomWalkTex.m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(randomWalkTex.m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//int textureSize = (gridSize - 1) * tileRes;
	int textureSize = walkGridSize;

	// Initialise meshs/ textures/ materials
	//==========================================================================
	Mesh cubeMesh;
	cubeMesh.CreateCubeMesh();
	Mesh terrainMesh;
	//terrainMesh.CreateFromHeightMap(&perlinTex, textureSize, textureSize);
	terrainMesh.CreateFromHeightMap(&perlinTex, textureSize, textureSize, 2);
	Mesh terrainMesh1;
	terrainMesh1.CreateFromHeightMap(&perlinTex, textureSize, textureSize);

	Texture blank(glm::vec3{ 0.7f, 0.7f, 0.7f });
	Texture blankNormal(glm::vec3{ 0.5f, 0.5f, 1.0f });

	Material defaultMat(&shaderSunOnly, &perlinTex, &blank, &blankNormal);
	defaultMat.SetLightProperties(0.1f, 1.0f, 0.5f);

	Material lightMatTest(&shaderScreenspace);
	//==========================================================================

	// Create game objects
	//==========================================================================
	GameObject terrain(&terrainMesh, &defaultMat);
	terrain.m_pos = glm::vec3(-textureSize, -2, -textureSize);
	GameObject terrain1(&terrainMesh1, &defaultMat);
	terrain1.m_pos = glm::vec3(0, -2, -textureSize);

	Camera cam({ 0, 3.0f, 10.0f });
	app->SetCurrentCamera(&cam);
	//==========================================================================

	app->SetUniformInAllShaders("sunDirection", glm::normalize(sunDirection));
	app->SetUniformInAllShaders("sunColour", sunColour);
	app->ApplyAllUniforms();
	float lastFrameTime = (float)glfwGetTime();

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
		//ImGui::Begin("DEBUG MENU");
		//ImGui::End();

		ImGui::Render();
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