#include "Application.h"
#include "Camera.h"
#include "PointLight.h"

#include <imgui.h>
#include "PoissonDisk.h"
#include "MapGeneration.h"

#include <iostream>
#include <random>

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


	// Procedural Generation Setup
	//==========================================================================
	const int maxSeed = 10000;
	int objectSeed = time(0) % maxSeed;
	//objectSeed = 2;

	HeightMap hMap = GenerateRandomWalk(100, 100, 20000, 0.1f);

	ObjectType tree;
	tree.rad = 0.5f;
	tree.exclusionRad = 10.0f;
	tree.minOverlap = 0.0f;
	tree.maxOverlap = 0.5f;
	tree.maxRotation = 3.5f;
	std::vector<ObjectInstance*> objects = PopulateMap(tree, hMap, 8, objectSeed);
	//==========================================================================


	// Initialise shaders
	//==========================================================================
	Shader vertShader1("shader1Vert", GL_VERTEX_SHADER);
	Shader fragShader1("shader1Frag", GL_FRAGMENT_SHADER);
	Shader fragShaderUnlit("shaderUnlitFrag", GL_FRAGMENT_SHADER);

	ShaderProgram shaderSunOnly(&vertShader1, &fragShader1);
	ShaderProgram shaderUnlitOnly(&vertShader1, &fragShaderUnlit);

	shaderSunOnly.m_uniforms.SetUniform("specPower", 10.0f);
	//==========================================================================


	// Initialise meshs/ textures/ materials
	//==========================================================================
	Mesh cubeMesh;
	cubeMesh.CreateCubeMesh();
	Mesh terrainMesh;
	terrainMesh.CreateFromHeightMap(hMap);
	Mesh treeMesh1;
	treeMesh1.LoadFromFile("Tree1.obj");
	Mesh treeMesh2;
	treeMesh2.LoadFromFile("Tree2.obj");

	Texture blank(glm::vec3{ 0.7f, 0.7f, 0.7f });
	Texture blankNormal(glm::vec3{ 0.5f, 0.5f, 1.0f });
	Texture redTex(glm::vec3{ 1.0f, 0.0f, 0.0f });
	Texture blueTex(glm::vec3{ 0.0f, 0.0f, 1.0f });
	Texture hMapTex(hMap);
	glTextureParameteri(hMapTex.m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(hMapTex.m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	Material defaultMat(&shaderSunOnly, &blank, &blank, &blankNormal);
	Material redMat(&shaderSunOnly, &redTex, &blank, &blankNormal);
	Material blueMat(&shaderSunOnly, &blueTex, &blank, &blankNormal);
	Material hMapMat(&shaderUnlitOnly, &hMapTex, &blank, &blankNormal);
	//==========================================================================


	// Create game objects
	//==========================================================================
	GameObject cube(&terrainMesh, &hMapMat);

	std::vector<Mesh*> meshVars = { &treeMesh1, &treeMesh2, &treeMesh2 };
	std::vector<Material*> matVars = { &defaultMat, &redMat, &blueMat };

	std::mt19937 twister(objectSeed);
	std::uniform_int_distribution<int> randomVar(0, 2);

	std::vector<GameObject*> allTrees;
	for (ObjectInstance* instance : objects)
	{
		int index = randomVar(twister);
		GameObject* tree = new GameObject(meshVars[index], matVars[index]);
		tree->m_pos = glm::vec3(instance->pos.x, instance->pos.y, instance->pos.z);
		tree->m_rot = glm::vec3(instance->rot.x, instance->rot.y, instance->rot.z);
		tree->m_scale = glm::vec3(0.5f);

		allTrees.push_back(tree);
	}

	Camera cam({ 50, 3.0f, 110.0f });
	cam.m_moveSpeed = 20;
	app->SetCurrentCamera(&cam);
	//==========================================================================


	// ImGui variables
	//==========================================================================
	bool overrideSeed = false;

	int spawnAttempts = 8;
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

		if (objects.size() > 0)
		{
			if (ImGui::Button("Clear objects"))
			{
				for (ObjectInstance* o : objects)
				{
					delete o;
				}
				objects.clear();

				for (GameObject* t : allTrees)
				{
					delete t;
				}
				allTrees.clear();
			}
		}
		else
		{
			if (ImGui::Button("Spawn objects"))
			{
				if (!overrideSeed)
				{
					objectSeed = (objectSeed + time(0)) % maxSeed;
				}

				objects = PopulateMap(tree, hMap, 8, objectSeed);
				std::mt19937 twister2(objectSeed);
				for (ObjectInstance* instance : objects)
				{
					int index = randomVar(twister2);
					GameObject* tree = new GameObject(meshVars[index], matVars[index]);
					tree->m_pos = glm::vec3(instance->pos.x, instance->pos.y, instance->pos.z);
					tree->m_rot = glm::vec3(instance->rot.x, instance->rot.y, instance->rot.z);
					tree->m_scale = glm::vec3(0.5f);

					allTrees.push_back(tree);
				}
			}
			ImGui::Checkbox("Override seed", &overrideSeed);
			ImGui::Dummy({ 0, 15 });

			ImGui::SliderFloat("Radius", &tree.rad, 0.0f, 10.0f);
			ImGui::SliderFloat("Exclusion radius", &tree.exclusionRad, 0.0f, 20.0f);
			ImGui::SliderInt("Spawn attempts", &spawnAttempts, 0, 20);
			ImGui::SliderFloat("Min overlap", &tree.minOverlap, -5.0f, 5.0f);
			tree.minOverlap = fmin(tree.minOverlap, tree.maxOverlap);
			ImGui::SliderFloat("Max overlap", &tree.maxOverlap, -5.0f, 5.0f);
			tree.maxOverlap = fmax(tree.minOverlap, tree.maxOverlap);
			ImGui::SliderAngle("MaxRotation", &tree.maxRotation);
		}

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