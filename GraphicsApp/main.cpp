#include "Application.h"
#include "Camera.h"
#include "PointLight.h"

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
		std::cout << "ERROR(main): Application could not inititalize\n";
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

	// Initialise meshs/ textures/ materials
	//==========================================================================
	Mesh spearMesh;
	spearMesh.LoadFromFile("soulspear.obj");
	Mesh cubeMesh;
	cubeMesh.CreateCubeMesh();
	Mesh planeMesh;
	planeMesh.CreatePlaneMesh();

	Texture blank(glm::vec3{ 0.7f, 0.7f, 0.7f });
	Texture blankNormal(glm::vec3{ 0.5f, 0.5f, 1.0f });
	Texture albedo("soulspear_diffuse.tga");
	Texture specular("soulspear_specular.tga");
	Texture normal("soulspear_normal.tga");

	Material defaultMat(&shaderAllLights, &blank, &blank, &blankNormal);
	defaultMat.SetLightProperties(0.1f, 1.0f, 0.5f);
	Material unlitMat(&shaderUnlit, &albedo, &specular, &normal);
	Material mat1(&shaderSunOnly, &albedo, &specular, &normal);
	Material mat2(&shaderAllLights, &albedo, &specular, &normal);
	mat2.SetLightProperties(0.1f, 1.0f, 1.0f);

	Material lightMat(&shaderScreenspace);
	//==========================================================================

	// Create game objects
	//==========================================================================
	GameObject spear1(&spearMesh, &unlitMat);
	spear1.m_pos = { -4.5f, 0, 0 };
	GameObject spear2(&spearMesh, &mat1);
	spear2.m_pos = { -1.5f, 0, 0 };
	GameObject spear3(&spearMesh, &defaultMat);
	spear3.m_pos = { 1.5f, 0, 0 };
	GameObject spear4(&spearMesh, &mat2);
	spear4.m_pos = { 4.5f, 0, 0 };
	GameObject cube1(&cubeMesh, &defaultMat);

	Camera cam({ 0, 3.0f, 10.0f });
	app->SetCurrentCamera(&cam);

	PointLight light1(&planeMesh, &lightMat);
	light1.m_pos = { -2.0f, 0, 0 };
	light1.SetColour({ 1, 0, 0 }, 5);
	PointLight light2(&planeMesh, &lightMat);
	light2.m_pos = { 2.0f, -0.5f, 1.5f };
	light2.SetColour({ 0, 1, 0 }, 5);
	PointLight light3(&planeMesh, &lightMat);
	light3.m_pos = { 3.0f, 0.5f, -0.5f };
	light3.SetColour({ 0, 0, 1 }, 5);
	//==========================================================================

	// Light logic setup
	//==========================================================================
	std::vector<PointLight*> lights;
	lights.push_back(&light1);
	lights.push_back(&light2);
	lights.push_back(&light3);

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColours;
	for (int i = 0; i < lights.size(); i++)
	{
		lightPositions.push_back(lights[i]->m_pos);
		lightColours.push_back(lights[i]->GetColour());
	}
	shaderAllLights.m_uniforms.SetUniform("lightCount", (int)lights.size());

	int selectedLight = 0;
	//==========================================================================

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
		shaderAllLights.Use();
		shaderAllLights.BindUniform("pointLightPos", lightPositions);
		shaderAllLights.BindUniform("pointLightCol", lightColours);
		//==========================================================================


		// DO IMGUI STUFF
		//==========================================================================
		// Must be before app.Draw(), as the info needs to be stored using ImGui::Render() before its actually drawn
		ImGui::Begin("DEBUG MENU");
		ImGui::BeginTabBar("Lights");

		if (ImGui::BeginTabItem("Light controls"))
		{
			ImGui::SliderInt("Selected Light", &selectedLight, 0, int(lights.size() - 1));
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