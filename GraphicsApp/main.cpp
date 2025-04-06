#include "GraphicsHeaders.h"

#include "Colour.h"
#include "Utilities.h"
#include "ShaderProgram.h"
#include "ext/matrix_transform.hpp"
//#include "ext/matrix_clip_space.hpp"
#include <ext.hpp>
//#include <glm.hpp>

#include "Application.h"
#include "Material.h"
#include "Camera.h"
#include "PointLight.h"

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//#include <stdio.h>

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

	glm::vec3 sunDirection = { 0, -1, -1 };

	Shader vertShader1("shader1Vert", GL_VERTEX_SHADER);
	Shader fragShader1("shader1Frag", GL_FRAGMENT_SHADER);
	Shader fragShader2("shader2Frag", GL_FRAGMENT_SHADER);
	Shader fragShaderUnlit("shaderUnlitFrag", GL_FRAGMENT_SHADER);
	Shader fragShaderTest("shaderTestFrag", GL_FRAGMENT_SHADER);

	ShaderProgram shader1(&vertShader1, &fragShader1);
	ShaderProgram shader2(&vertShader1, &fragShaderUnlit);
	ShaderProgram shader3(&vertShader1, &fragShader2);
	ShaderProgram shaderNormal(&vertShader1, &fragShaderTest);
	shader1.m_uniformFloats["specPower"] = 2;
	shader1.m_uniformVec3s["sunDirection"] = sunDirection;
	shader3.m_uniformFloats["specPower"] = 2;
	shader3.m_uniformVec3s["sunDirection"] = sunDirection;
	shader3.m_uniformVec3s["sunColour"] = { 1, 1, 1 };

	Camera* cam = new Camera({ 0, 3.0f, 10.0f });
	//cam->Init(&app);
	app.AddObject(cam);
	app.SetCurrentCamera(cam);
	//cam.m_yRot = glm::radians(90.0f);

	glEnable(GL_DEPTH_TEST); // Enables depth buffer

	Mesh spearMesh;
	spearMesh.LoadFromFile("soulspear.obj");
	Mesh cubeMesh;
	cubeMesh.CreateCubeMesh();

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
	defaultMat.m_specular = 0.1f;

	Material mat1(&shader1, &albedo, &specular, &normal);
	Material mat2(&shader2, &albedo, &specular, &normal);
	Material mat3(&shader1, &normal, &normal, &normal);
	Material mat4(&shader3, &albedo, &specular, &normal);
	mat1.m_specular = 1.0f;
	mat3.m_specular = 1.0f;
	mat4.m_specular = 1.0f;

	GameObject* spear1 = new GameObject();
	spear1->m_mesh = &spearMesh;
	spear1->m_mat = &mat1;
	spear1->m_pos = { -1.5f, 0, 0 };
	app.AddObject(spear1);

	GameObject* spear2 = new GameObject();
	spear2->m_mesh = &spearMesh;
	spear2->m_mat = &mat2;
	spear2->m_pos = { 4.5f, 0, 0 };
	app.AddObject(spear2);

	GameObject* spear3 = new GameObject();
	spear3->m_mesh = &spearMesh;
	spear3->m_mat = &mat3;
	spear3->m_pos = { -4.5f, 0, 0 };
	app.AddObject(spear3);

	GameObject* spear4 = new GameObject();
	spear4->m_mesh = &spearMesh;
	spear4->m_mat = &mat4;
	spear4->m_pos = { 1.5f, 0, 0 };
	app.AddObject(spear4);

	GameObject* cube1 = new GameObject();
	cube1->m_mesh = &cubeMesh;
	cube1->m_mat = &defaultMat;
	cube1->m_pos = { 0, 0, 0 };
	//cube1->m_scale = { 0.1f, 0.1f, 0.1f};
	app.AddObject(cube1);

	std::vector<PointLight> lights;
	PointLight light1({ 0.0f, 0.0f, 0.0f }, { 1, 0, 0 }, 10);
	PointLight light2({ 2.0f, 0.0f, 1.0f }, { 1, 0, 0 }, 10);
	PointLight light3({ 3.0f, 0.5f, -0.5f }, { 1, 0, 0 }, 10);
	PointLight light4({ 0.0f, 1.0f, 1.0f }, { 1, 0, 0 }, 10);
	PointLight light5({ -2.0f, 0.0f, 1.0f }, { 0, 0, 1 }, 10);
	lights.push_back(light1);
	//lights.push_back(light2);
	//lights.push_back(light3);
	//lights.push_back(light4);
	//lights.push_back(light5);

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColours;
	for (int i = 0; i < lights.size(); i++)
	{
		lightPositions.push_back(lights[i].pos);
		lightColours.push_back(lights[i].col);
	}

	shader3.m_uniformInts["lightCount"] = lights.size();
	shader3.m_uniformVec3Arrays["pointLightPos"] = lightPositions;
	shader3.m_uniformVec3Arrays["pointLightCol"] = lightColours;

	glm::vec3 testColour = light1.col;


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
		if (app.GetKeyDown(GLFW_KEY_X))
		{
			app.ReloadShaders();
		}

		// Factors in camera position, but not rotation
		//glm::mat4 test = app.GetProjectionMatrix() * glm::translate(glm::mat4(1), -app.GetCurrentCamera()->GetPos());

		glm::mat4 vpMat = app.GetVPMatrix();
		glm::vec3 camPos = app.GetCurrentCamera()->GetPos();

		app.SetUniformInAllShaders("cameraPos", camPos);
		app.SetUniformInAllShaders("vpMat", vpMat);

		for (int i = 0; i < lights.size(); i++)
		{
			glm::vec4 transformedPos = { lights[i].pos.x, lights[i].pos.y, lights[i].pos.z, 1 };
			transformedPos = vpMat * transformedPos;
			lightPositions[i] = { transformedPos.x, transformedPos.y, transformedPos.z };

			lightColours[i] = testColour;
		
			//glm::vec4 test = { spear4->m_pos.x, spear4->m_pos.y, spear4->m_pos.z, 1 };
			//test = vpMat * test;
			//std::cout << "LIGHT: " << transformedPos.x << ", " << transformedPos.y << ", " << transformedPos.z << std::endl;
			//std::cout << "SPEAR: " << test.x << ", " << test.y << ", " << test.z << std::endl;
		}
		shader3.m_uniformVec3Arrays["pointLightPos"] = lightPositions;
		shader3.m_uniformVec3Arrays["pointLightCol"] = lightColours;

		// DO IMGUI STUFF
		//==========================================================================
		// Must be before app.Draw(), as the info needs to be stored using ImGui::Render() before its actually drawn
		ImGui::Begin("DEBUG MENU");

		ImGui::ColorEdit3("Point light 1 colour", glm::value_ptr(light1.col));

		ImGui::End();

		ImGui::Render();
		//==========================================================================

		app.Draw();

		// END OF FRAME
		//==========================================================================
		glfwSwapBuffers(app.GetWindow()); // Displays buffer we just wrote to 
		glfwPollEvents(); // Check for inputs
		//==========================================================================
	}

	glfwTerminate();
	return 0;
}