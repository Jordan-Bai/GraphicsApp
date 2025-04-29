#include "GraphicsHeaders.h"

#include "Colour.h"
#include "Utilities.h"
#include "ShaderProgram.h"
#include "ext/matrix_transform.hpp"
#include <ext.hpp>

#include "Application.h"
#include "Material.h"
#include "Camera.h"
#include "PointLight.h"

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <vector>

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
	//Shader fragShaderTest("shaderTestFrag", GL_FRAGMENT_SHADER);
	Shader vertShaderScreenspace("shaderScreenspaceVert", GL_VERTEX_SHADER);
	Shader fragShaderScreenspace("shaderScreenspaceFrag", GL_FRAGMENT_SHADER);

	ShaderProgram shader1(&vertShader1, &fragShader1);
	ShaderProgram shader2(&vertShader1, &fragShaderUnlit);
	ShaderProgram shader3(&vertShader1, &fragShader2);
	//ShaderProgram shaderNormal(&vertShader1, &fragShaderTest);
	ShaderProgram shaderScreenspace(&vertShaderScreenspace, &fragShaderScreenspace);

	shader1.m_uniforms.SetUniform("specPower", 10.0f);
	shader1.m_uniforms.SetUniform("sunDirection", glm::normalize(sunDirection));
	shader3.m_uniforms.SetUniform("specPower", 10.0f);
	shader3.m_uniforms.SetUniform("sunDirection", glm::normalize(sunDirection));
	shader3.m_uniforms.SetUniform("sunColour", sunColour);
	shaderScreenspace.m_uniforms.SetUniform("aspectRatio", app->GetAspectRatio());
	//==========================================================================

	// Initialise meshs/ textures/ materials
	//==========================================================================
	Mesh spearMesh;
	spearMesh.LoadFromFile("soulspear.obj");
	Mesh cubeMesh;
	cubeMesh.CreateCubeMesh();
	Mesh planeMesh;
	planeMesh.CreatePlaneMesh();

	Texture blank(glm::vec3{ 0.7f, 0.7f, 0.7f });
	//blank.CreateColourTexture({0.7f, 0.7f, 0.7f});
	Texture blankNormal(glm::vec3{ 0.5f, 0.5f, 1.0f });
	//blankNormal.CreateColourTexture({ 0.5f, 0.5f, 1.0f });
	Texture albedo("soulspear_diffuse.tga");
	//albedo.LoadFileAsTexture("soulspear_diffuse.tga");
	Texture specular("soulspear_specular.tga");
	//specular.LoadFileAsTexture("soulspear_specular.tga");
	Texture normal("soulspear_normal.tga");
	//normal.LoadFileAsTexture("soulspear_normal.tga");

	Material defaultMat(&shader3, &blank, &blank, &blankNormal);
	defaultMat.SetLightProperties(0.1f, 1.0f, 0.5f);
	Material unlitMat(&shader2, &albedo, &specular, &normal);
	Material mat1(&shader1, &albedo, &specular, &normal);
	Material mat2(&shader3, &albedo, &specular, &normal);
	mat2.SetLightProperties(0.1f, 1.0f, 1.0f);

	Material lightMatTest(&shaderScreenspace);
	//==========================================================================

	// Create game objects
	//==========================================================================
	GameObject spear1(&spearMesh, &unlitMat);
	spear1.m_pos = { -4.5f, 0, 0 };
	GameObject spear2(&spearMesh, &defaultMat);
	spear2.m_pos = { -1.5f, 0, 0 };
	GameObject spear3(&spearMesh, &mat2);
	spear3.m_pos = { 1.5f, 0, 0 };
	GameObject spear4(&spearMesh, &mat2);
	spear4.m_pos = { 4.5f, 0, 0 };
	GameObject cube1(&cubeMesh, &defaultMat);

	Camera cam({ 0, 3.0f, 10.0f });
	app->SetCurrentCamera(&cam);

	std::vector<PointLight*> lights;
	PointLight light1(&planeMesh, &lightMatTest);
	light1.m_pos = { -2.0f, 0, 0 };
	light1.SetColour({ 1, 0, 0 }, 10);
	PointLight light2(&planeMesh, &lightMatTest);
	light2.m_pos = { 2.0f, 0, 1.0f };
	light2.SetColour({ 0, 1, 0 }, 10);
	PointLight light3(&planeMesh, &lightMatTest);
	light3.m_pos = { 3.0f, 0.5f, -0.5f };
	light3.SetColour({ 0, 0, 1 }, 10);
	lights.push_back(&light1);
	lights.push_back(&light2);
	lights.push_back(&light3);
	//==========================================================================

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColours;
	for (int i = 0; i < lights.size(); i++)
	{
		lightPositions.push_back(lights[i]->m_pos);
		lightColours.push_back(lights[i]->GetColour());
	}
	shader3.m_uniforms.SetUniform("lightCount", (int)lights.size());

	int selectedLight = 0;

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
		app->BindUniformInAllShaders("sunDirection", glm::normalize(sunDirection));
		app->BindUniformInAllShaders("sunColour", sunColour);

		int i = selectedLight;
		lightPositions[i] = lights[i]->m_pos;
		lightColours[i] = lights[i]->GetColour();
		shader3.Use();
		shader3.BindArrayUniform("pointLightPos", lightPositions);
		shader3.BindArrayUniform("pointLightCol", lightColours);
		//int i = selectedLight;
		//shader3.Use();
		//shader3.BindArrayElementUniform("pointLightPos", i, lights[i]->pos);
		//shader3.BindArrayElementUniform("pointLightCol", i, lights[i]->GetColour());
		//shader3.m_uniforms.SetArrayElementUniform("pointLightPos", i, lights[i]->pos);
		//shader3.m_uniforms.SetArrayElementUniform("pointLightCol", i, lights[i]->GetColour());
		//==========================================================================


		// DO IMGUI STUFF
		//==========================================================================
		// Must be before app.Draw(), as the info needs to be stored using ImGui::Render() before its actually drawn
		ImGui::Begin("DEBUG MENU");
		ImGui::BeginTabBar("Lights");

		if (ImGui::BeginTabItem("Light controls"))
		{
			ImGui::SliderInt("Selected Light", &selectedLight, 0, lights.size() - 1);
			ImGui::Dummy({ 0, 15 });
			ImGui::ColorEdit3("Colour", glm::value_ptr(lights[selectedLight]->m_col));
			ImGui::SliderFloat("Brightness", &lights[selectedLight]->m_bright, 0, 50.0f);
			ImGui::SliderFloat3("Position", glm::value_ptr(lights[selectedLight]->m_pos), -10.0f, 10.0f);
			ImGui::EndTabItem();
		}
		
		if (ImGui::BeginTabItem("Sun controls"))
		{
			ImGui::SliderFloat3("Direction", glm::value_ptr(sunDirection), -1.0f, 1.0f);
			ImGui::ColorEdit3("Colour", glm::value_ptr(sunColour));
			ImGui::EndTabItem();
		}
		
		ImGui::EndTabBar();
		ImGui::End();

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