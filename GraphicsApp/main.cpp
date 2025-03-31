#include "GraphicsHeaders.h"

#include "Colour.h"
#include "Utilities.h"
#include "ShaderProgram.h"
#include "ext/matrix_transform.hpp"
//#include "ext/matrix_clip_space.hpp"

#include "Application.h"
#include "Material.h"
#include "Camera.h"
#include "PointLight.h"

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

	//glEnableVertexAttribArray(0); // layout location 0 in the vertex shader
	//glEnableVertexAttribArray(1); // layout location 1 in the vertex shader
	//glEnableVertexAttribArray(2); // layout location 2 in the vertex shader
	//glEnableVertexAttribArray(3); // layout location 2 in the vertex shader
	ShaderProgram shader1("shader1Vert", "shader1Frag");
	ShaderProgram shader2("shader1Vert", "shaderUnlitFrag");
	ShaderProgram shader3("shader1Vert", "shader2Frag");
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

	Texture albedo;
	albedo.LoadFileAsTexture("soulspear_diffuse.tga");
	Texture specular;
	specular.LoadFileAsTexture("soulspear_specular.tga");
	Texture normal;
	normal.LoadFileAsTexture("soulspear_normal.tga");

	Material mat1(&shader1, &albedo, &specular, &normal);
	Material mat2(&shader2, &albedo, &specular, &normal);
	Material mat3(&shader1, &normal, &normal, &normal);
	Material mat4(&shader3, &albedo, &specular, &normal);

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

	std::vector<PointLight> lights;
	PointLight light1({0.0f, 0.0f, 0.0f}, {1, 0, 0}, 10);
	lights.push_back(light1);
	//lights.push_back(light1);

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
			shader3.ReloadShader();
		}

		glm::mat4 vpMat = app.GetVPMatrix();
		glm::vec3 camPos = app.GetCurrentCamera()->GetPos();

		shader1.m_uniformVec3s["cameraPos"] = camPos;
		shader1.m_uniformMat4s["vpMat"] = vpMat;
		shader2.m_uniformVec3s["cameraPos"] = camPos;
		shader2.m_uniformMat4s["vpMat"] = vpMat;
		shader3.m_uniformVec3s["cameraPos"] = camPos;
		shader3.m_uniformMat4s["vpMat"] = vpMat;

		for (int i = 0; i < lights.size(); i++)
		{
			glm::vec4 transformedPos = { lights[i].pos.x, lights[i].pos.y, lights[i].pos.z, 1 };
			transformedPos = vpMat * transformedPos;
			lightPositions[i] = { transformedPos.x, transformedPos.y, transformedPos.z };
		
			//glm::vec4 test = { spear4->m_pos.x, spear4->m_pos.y, spear4->m_pos.z, 1 };
			//test = vpMat * test;
			//std::cout << "LIGHT: " << transformedPos.x << ", " << transformedPos.y << ", " << transformedPos.z << std::endl;
			//std::cout << "SPEAR: " << test.x << ", " << test.y << ", " << test.z << std::endl;
		}
		shader3.m_uniformVec3Arrays["pointLightPos"] = lightPositions;

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