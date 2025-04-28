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

	glm::vec3 sunDirection = { 0, -1, -1 };

	//glEnableVertexAttribArray(0); // layout location 0 in the vertex shader
	//glEnableVertexAttribArray(1); // layout location 1 in the vertex shader
	//glEnableVertexAttribArray(2); // layout location 2 in the vertex shader
	//glEnableVertexAttribArray(3); // layout location 2 in the vertex shader

	Shader vertShader1("shader1Vert", GL_VERTEX_SHADER);
	Shader vertShaderUI("shaderUIVert", GL_VERTEX_SHADER);
	Shader fragShader1("shader1Frag", GL_FRAGMENT_SHADER);
	Shader fragShader2("shader2Frag", GL_FRAGMENT_SHADER);
	Shader fragShaderUnlit("shaderUnlitFrag", GL_FRAGMENT_SHADER);
	Shader fragShaderUI("shaderUIFrag", GL_FRAGMENT_SHADER);
	Shader fragShaderTest("shaderTestFrag", GL_FRAGMENT_SHADER);

	ShaderProgram shader1(&vertShader1, &fragShader1);
	ShaderProgram shader2(&vertShader1, &fragShaderUnlit);
	ShaderProgram shader3(&vertShader1, &fragShader2);
	ShaderProgram shaderNormal(&vertShader1, &fragShaderTest);
	ShaderProgram shaderUI(&vertShaderUI, &fragShaderUI);

	shader1.m_uniforms.SetUniform("specPower", 2.0f);
	shader1.m_uniforms.SetUniform("sunDirection", sunDirection);
	shader3.m_uniforms.SetUniform("specPower", 2.0f);
	shader3.m_uniforms.SetUniform("sunDirection", sunDirection);
	shader3.m_uniforms.SetUniform("sunColour", { 1, 1, 1 });
	shaderUI.m_uniforms.SetUniform("aspectRatio", app->GetAspectRatio());

	Camera cam({ 0, 3.0f, 10.0f });
	app->AddObject(&cam);
	app->SetCurrentCamera(&cam);

	//glEnable(GL_DEPTH_TEST); // Enables depth buffer

	Mesh spearMesh;
	spearMesh.LoadFromFile("soulspear.obj");
	Mesh cubeMesh;
	cubeMesh.CreateCubeMesh();
	Mesh planeMesh;
	planeMesh.CreatePlaneMesh();

	Texture blank;
	blank.CreateColourTexture({0.7f, 0.7f, 0.7f});
	Texture blankNormal;
	blankNormal.CreateColourTexture({ 0.5f, 0.5f, 1.0f });
	Texture albedo;
	albedo.LoadFileAsTexture("soulspear_diffuse.tga");
	Texture specular;
	specular.LoadFileAsTexture("soulspear_specular.tga");
	Texture normal;
	normal.LoadFileAsTexture("soulspear_normal.tga");

	Material defaultMat(&shader3, &blank, &blank, &blankNormal);
	defaultMat.SetLightProperties(0.1f, 1.0f, 0.1f);
	Material mat1(&shader1, &albedo, &specular, &normal);
	Material mat2(&shader2, &albedo, &specular, &normal);
	Material mat3(&shader1, &normal, &normal, &normal);
	Material mat4(&shader3, &albedo, &specular, &normal);
	mat4.SetLightProperties(0.1f, 1.0f, 1.0f);

	Material lightMat1(&shaderUI);
	Material lightMat2(&shaderUI);
	Material lightMat3(&shaderUI);

	GameObject spear1(&spearMesh, &mat1);
	spear1.m_pos = { -1.5f, 0, 0 };
	app->AddObject(&spear1);
	GameObject spear2(&spearMesh, &mat2);
	spear2.m_pos = { 4.5f, 0, 0 };
	app->AddObject(&spear2);
	GameObject spear3(&spearMesh, &mat3);
	spear3.m_pos = { -4.5f, 0, 0 };
	app->AddObject(&spear3);
	GameObject spear4(&spearMesh, &mat4);
	spear4.m_pos = { 1.5f, 0, 0 };
	app->AddObject(&spear4);

	GameObject cube1(&cubeMesh, &defaultMat);
	app->AddObject(&cube1);


	GameObject plane1(&planeMesh, &lightMat1);
	app->AddObject(&plane1);
	GameObject plane2(&planeMesh, &lightMat2);
	app->AddObject(&plane2);
	GameObject plane3(&planeMesh, &lightMat3);
	app->AddObject(&plane3);

	std::vector<PointLight*> lights;
	PointLight light1({ -2.0f, 0.0f, 0.0f }, { 1, 0, 0 }, 10);
	PointLight light2({ 2.0f, 0.0f, 1.0f }, { 0, 1, 0 }, 10);
	PointLight light3({ 3.0f, 0.5f, -0.5f }, { 0, 0, 1 }, 10);
	lights.push_back(&light1);
	lights.push_back(&light2);
	lights.push_back(&light3);

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColours;
	for (int i = 0; i < lights.size(); i++)
	{
		lightPositions.push_back(lights[i]->pos);
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
		glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		//==========================================================================

		app->Update(delta);

		plane1.m_pos = light1.pos;
		plane2.m_pos = light2.pos;
		plane3.m_pos = light3.pos;

		lightMat1.m_uniforms.SetUniform("colour", light1.col);
		lightMat2.m_uniforms.SetUniform("colour", light2.col);
		lightMat3.m_uniforms.SetUniform("colour", light3.col);

		if (app->GetKeyDown(GLFW_KEY_X))
		{
			app->ReloadShaders();
		}

		glm::mat4 vpMat = app->GetVPMatrix();
		glm::vec3 camPos = app->GetCurrentCamera()->GetPos();

		app->BindUniformInAllShaders("cameraPos", camPos);
		app->BindUniformInAllShaders("vpMat", vpMat);

		for (int i = 0; i < lights.size(); i++)
		{
			glm::vec4 transformedPos = { lights[i]->pos.x, lights[i]->pos.y, lights[i]->pos.z, 1 };
			transformedPos = vpMat * transformedPos;
			lightPositions[i] = { transformedPos.x, transformedPos.y, transformedPos.z };

			lightColours[i] = lights[i]->GetColour();
		}
		shader3.Use();
		shader3.BindArrayUniform("pointLightPos", lightPositions);
		shader3.BindArrayUniform("pointLightCol", lightColours);

		// DO IMGUI STUFF
		//==========================================================================
		// Must be before app.Draw(), as the info needs to be stored using ImGui::Render() before its actually drawn
		ImGui::Begin("DEBUG MENU");

		ImGui::SliderInt("Selected Light", &selectedLight, 0, lights.size() - 1);
		ImGui::Dummy({ 0, 15 });
		ImGui::ColorEdit3("Colour", glm::value_ptr(lights[selectedLight]->col));
		ImGui::SliderFloat("Brightness", &lights[selectedLight]->bright, 0, 100.0f);
		ImGui::SliderFloat3("Position", glm::value_ptr(lights[selectedLight]->pos), -10.0f, 10.0f);

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