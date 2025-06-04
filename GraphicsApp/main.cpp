#include "Application.h"
#include "Camera.h"
#include "PointLight.h"
#include "PerlinNoise.h"
#include "RandomWalk.h"
#include "PoissonDisk.h"

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

	glm::vec3 sunDirection = { 1, -1, -0.75 };
	glm::vec3 sunColour = { 1, 1, 1 };

	// Initialise shaders
	//==========================================================================
	Shader vertShader1("shader1Vert", GL_VERTEX_SHADER);
	Shader fragShader1("shader1Frag", GL_FRAGMENT_SHADER);
	Shader fragShader2("shader2Frag", GL_FRAGMENT_SHADER);
	Shader fragShaderUnlit("shaderUnlitFrag", GL_FRAGMENT_SHADER);
	Shader vertShaderScreenspace("shaderScreenspaceVert", GL_VERTEX_SHADER);
	Shader fragShaderScreenspace("shaderScreenspaceFrag", GL_FRAGMENT_SHADER);
	Shader vertShaderBuffer("shaderBufferVert", GL_VERTEX_SHADER);
	Shader fragShaderBuffer("shaderBufferFrag", GL_FRAGMENT_SHADER);

	ShaderProgram shaderSunOnly(&vertShader1, &fragShader1);
	ShaderProgram shaderAllLights(&vertShader1, &fragShader2);
	ShaderProgram shaderUnlit(&vertShader1, &fragShaderUnlit);
	ShaderProgram shaderScreenspace(&vertShaderScreenspace, &fragShaderScreenspace);
	ShaderProgram shaderBuffer(&vertShaderBuffer, &fragShaderBuffer);

	shaderSunOnly.m_uniforms.SetUniform("specPower", 10.0f);
	shaderSunOnly.m_uniforms.SetUniform("sunDirection", glm::normalize(sunDirection));
	shaderSunOnly.m_uniforms.SetUniform("sunColour", sunColour);
	shaderAllLights.m_uniforms.SetUniform("specPower", 10.0f);
	shaderScreenspace.m_uniforms.SetUniform("aspectRatio", app->GetAspectRatio());
	shaderBuffer.m_uniforms.SetUniform("aspectRatio", app->GetAspectRatio());
	//==========================================================================
	const int seedMax = 100000;
	int heightSeed = time(0) % seedMax;
	int populateSeed = time(0) % seedMax;
	// FOR TESTING
	heightSeed = 1;
	populateSeed = 1;

	const int walkGridSize = 100;
	int steps = 20000;
	Texture randomWalkTex = GenerateWalk(walkGridSize, steps, heightSeed);
	//randomWalkTex.BlurTexture(2, 0.5f);
	glTextureParameteri(randomWalkTex.m_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(randomWalkTex.m_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Initialise meshs/ textures/ materials
	//==========================================================================
	Mesh cubeMesh;
	cubeMesh.CreateCubeMesh();
	Mesh quadMesh;
	quadMesh.CreatePlaneMesh();
	Mesh treeMesh1;
	treeMesh1.LoadFromFile("Tree1.obj");
	Mesh treeMesh2;
	treeMesh2.LoadFromFile("Tree2.obj");
	Mesh terrainMesh;
	terrainMesh.CreateFromHeightMap(&randomWalkTex, walkGridSize, walkGridSize);

	Texture blank(glm::vec3(0.7f));
	Texture blankNormal(glm::vec3{ 0.5f, 0.5f, 1.0f });
	Texture red(glm::vec3{ 1.0f, 0.0f, 0.0f });

	Material defaultMat(&shaderSunOnly, &blank, &blank, &blankNormal);
	defaultMat.SetLightProperties(0.1f, 1.0f, 0.5f);
	Material blueMat(&shaderSunOnly, &blankNormal, &blankNormal, &blankNormal);
	blueMat.SetLightProperties(0.1f, 1.0f, 0.5f);
	Material redMat(&shaderSunOnly, &red, &red, &blankNormal);
	redMat.SetLightProperties(0.1f, 1.0f, 0.5f);
	Material walkMat(&shaderUnlit, &randomWalkTex, &blank, &blankNormal);

	Material previewMat(&shaderBuffer, &blank, &blank, &blankNormal);
	//==========================================================================

	// Create game objects
	//==========================================================================
	GameObject terrain(&terrainMesh, &walkMat);

	Variant* treeVar1 = new Variant(&cubeMesh, &defaultMat);
	Variant* treeVar2 = new Variant(&cubeMesh, &blueMat);
	Variant* treeVar3 = new Variant(&cubeMesh, &redMat);

	Variant* selectedVar = treeVar1;

	ObjectType trees;
	trees.rad = 0.5;
	trees.exclusionRad = 8;
	trees.spawnAttempts = 8;
	trees.objectVariants.push_back(treeVar1);
	trees.objectVariants.push_back(treeVar2);
	trees.objectVariants.push_back(treeVar3);
	trees.minOverlap = 0;
	trees.maxOverlap = 0.1;
	trees.rotate = true;
	trees.maxRotation = 1.1;
	trees.scale = glm::vec3(0.5f);
	std::vector<GameObject*> boxes;
	boxes = PopulateMap(trees, randomWalkTex, populateSeed);

	Camera cam({ 50, 3.0f, 120.0f });
	app->SetCurrentCamera(&cam);
	//==========================================================================


	// TESTING FRAME BUFFER
	//==========================================================================
	glm::vec2 bufferSize = glm::vec2(app->GetWidth(), app->GetHeight());

	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferSize.x, bufferSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	Texture bufferTex(bufferSize);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferTex.m_texture, 0);
	previewMat.m_albedoMap = &bufferTex;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//==========================================================================
	GameObject variantPreview;
	GameObject minOverlapPreview(&quadMesh, &redMat);
	minOverlapPreview.m_rot = glm::vec3(-1.57075, 0, 0);
	GameObject maxOverlapPreview(&quadMesh, &blueMat);
	maxOverlapPreview.m_rot = glm::vec3(-1.57075, 0, 0);

	GameObject previewPlane(&quadMesh, &previewMat);
	previewPlane.m_pos = glm::vec3(1, 1, 0);
	previewPlane.m_scale *= 0.8f;

	Camera previewCam(glm::vec3(0, 2, 8));
	app->RemoveObject(&previewCam);
	app->RemoveObject(&variantPreview);
	app->RemoveObject(&minOverlapPreview);
	app->RemoveObject(&maxOverlapPreview);
	app->RemoveObject(&previewPlane);


	// VARIABLES FOR IMGUI SETTINGS
	//==========================================================================
	bool variantSettings = false;

	int selectedVarIndex = 0;

	std::vector<int> selectedMesh = { 0, 1, 1 };
	std::vector<Mesh*> availableMeshes = { &treeMesh1, &treeMesh2, &cubeMesh, &quadMesh };

	std::vector<int> selectedMat = {0, 1, 2};
	std::vector<Material*> availableMats = {&defaultMat, &blueMat, &redMat};

	bool overrideHeightSeed = false;
	bool overridePopSeed = true;

	int blurSize = 1;
	float blurAmount = 0;
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

		// FRAME BUFFER STUFF
		//==========================================================================
		if (selectedVar && boxes.size() == 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

			// Set background colour
			glClearColor(0.8f, 0.5f, 0.5f, 0.5f);
			// Clears the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			selectedVar = trees.objectVariants[selectedVarIndex];
			selectedVar->mat = availableMats[selectedMat[selectedVarIndex]];
			selectedVar->mesh = availableMeshes[selectedMesh[selectedVarIndex]];

			selectedVar->mat->m_shader->Use();
			selectedVar->mat->m_shader->BindUniform("cameraPos", previewCam.GetPos());
			selectedVar->mat->m_shader->BindUniform("vpMat", app->GetProjectionMatrix() * previewCam.GetViewMatrix());

			variantPreview.m_mat = selectedVar->mat;
			variantPreview.m_mesh = selectedVar->mesh;
			variantPreview.m_scale = trees.scale;
			if (trees.rotate)
			{
				variantPreview.m_rot.z = trees.maxRotation;
				minOverlapPreview.m_rot.z = trees.maxRotation;
				maxOverlapPreview.m_rot.z = trees.maxRotation;
				minOverlapPreview.m_pos = glm::vec3(-sin(trees.maxRotation), cos(trees.maxRotation), 0) * trees.minOverlap;
				maxOverlapPreview.m_pos = glm::vec3(-sin(trees.maxRotation), cos(trees.maxRotation), 0) * trees.maxOverlap;
			}
			else
			{
				variantPreview.m_rot.z = 0;
				minOverlapPreview.m_rot.z = 0;
				maxOverlapPreview.m_rot.z = 0;

				minOverlapPreview.m_pos = glm::vec3(0, trees.minOverlap, 0);
				maxOverlapPreview.m_pos = glm::vec3(0, trees.maxOverlap, 0);
			}
			variantPreview.Draw();

			// Draw min/ max overlap
			minOverlapPreview.m_mat->m_shader->Use();
			minOverlapPreview.m_mat->m_shader->BindUniform("cameraPos", previewCam.GetPos());
			minOverlapPreview.m_mat->m_shader->BindUniform("vpMat", app->GetProjectionMatrix() * previewCam.GetViewMatrix());

			//minOverlapPreview.m_scale = trees.scale + glm::vec3(0.75f);
			//maxOverlapPreview.m_scale = trees.scale + glm::vec3(0.75f);
			minOverlapPreview.m_scale = glm::vec3(trees.rad + 0.05f);
			maxOverlapPreview.m_scale = glm::vec3(trees.rad + 0.05f);
			minOverlapPreview.Draw();
			maxOverlapPreview.Draw();

			// Store the texture properly
			glBindTexture(GL_TEXTURE_2D, bufferTex.m_texture);
			glGenerateMipmap(GL_TEXTURE_2D); // Mip-mapping
		}
		//==========================================================================
		
		// CLEAR SCREEN
		//==========================================================================
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Set background colour
		glClearColor(0.5f, 0.5f, 0.8f, 0.5f);
		// Clears the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

		if (boxes.size() == 0)
		{
			if (ImGui::Button("Spawn Objects"))
			{
				if (!overridePopSeed)
				{
					populateSeed = (populateSeed + time(0)) % seedMax;
				}
				boxes = PopulateMap(trees, randomWalkTex, populateSeed);
			}
			ImGui::Dummy({ 0, 15 });
			ImGui::SliderFloat3("Preview cam pos", glm::value_ptr(previewCam.m_pos), 0.0f, 10.0f);
			ImGui::Dummy({ 0, 15 });

			ImGui::BeginTabBar("Spawning settings");
			// OBJECT TAB
			//==========================================================================
			if (ImGui::BeginTabItem("Object type"))
			{
				ImGui::SliderFloat("Exclusion radius", &trees.exclusionRad, 0.0f, 50.0f);
				ImGui::SliderInt("Spawn Attempts", &trees.spawnAttempts, 0, 20);
				// Min & Max overlap
				ImGui::SliderFloat("MinOverlap", &trees.minOverlap, -10.0f, 10.0f);
				trees.minOverlap = Min(trees.minOverlap, trees.maxOverlap);
				ImGui::SliderFloat("MaxOverlap", &trees.maxOverlap, -10.0f, 10.0f);
				trees.maxOverlap = Max(trees.minOverlap, trees.maxOverlap);
				// Rotation
				ImGui::Checkbox("Rotate", &trees.rotate);
				if (trees.rotate)
				{
					ImGui::SliderFloat("MaxRotation", &trees.maxRotation, 0.0f, 3.14159f / 2.0f);
				}

				ImGui::SliderFloat3("Scale", glm::value_ptr(trees.scale), 0.0f, 10.0f);
				ImGui::EndTabItem();
			}
			// VARIANT TAB
			//==========================================================================
			if (ImGui::BeginTabItem("Variant"))
			{
				if (ImGui::Button("Create new variant"))
				{
					Variant* var = new Variant(availableMeshes[0], availableMats[0]);
					trees.objectVariants.push_back(var);
					selectedMesh.push_back(0);
					selectedMat.push_back(0);
					selectedVarIndex = trees.objectVariants.size() - 1;
				}
				ImGui::SliderInt("Selected variant", &selectedVarIndex, 0, trees.objectVariants.size() - 1);
				ImGui::SliderInt("Selected mesh", &selectedMesh[selectedVarIndex], 0, availableMeshes.size() - 1);
				ImGui::SliderInt("Selected material", &selectedMat[selectedVarIndex], 0, availableMats.size() - 1);
				if (trees.objectVariants.size() > 1)
				{
					if (ImGui::Button("Delete variant"))
					{
						trees.objectVariants.erase(trees.objectVariants.begin() + selectedVarIndex);
						selectedMesh.erase(selectedMesh.begin() + selectedVarIndex);
						selectedMat.erase(selectedMat.begin() + selectedVarIndex);
						selectedVarIndex = Clamp(selectedVarIndex, 0, trees.objectVariants.size() - 1);
					}
				}
				ImGui::EndTabItem();
			}
			// GENERATION TAB
			//==========================================================================
			if (ImGui::BeginTabItem("Generation"))
			{
				ImGui::Checkbox("Override object seed", &overridePopSeed);
				if (overridePopSeed)
				{
					ImGui::SliderInt("Object seed", &populateSeed, 0, seedMax);
				}
				else
				{
					ImGui::LabelText("Object seed", std::to_string(populateSeed).c_str());
				}
				ImGui::Dummy({ 0, 15 });

				ImGui::Checkbox("Override heightmap seed", &overrideHeightSeed);
				if (overrideHeightSeed)
				{
					ImGui::SliderInt("Heightmap seed", &heightSeed, 0, seedMax);
				}
				else
				{
					ImGui::LabelText("Heightmap seed", std::to_string(heightSeed).c_str());
				}
				ImGui::SliderFloat("Blur amount", &blurAmount, 0.0f, 1.0f);
				ImGui::SliderInt("Blur size", &blurSize, 1, 50);
				if (ImGui::Button("Regenerate map"))
				{
					if (!overrideHeightSeed)
					{
						heightSeed = (heightSeed + time(0)) % seedMax;
					}
					randomWalkTex = GenerateWalk(walkGridSize, steps, heightSeed);
					if (blurAmount > 0)
					{
						randomWalkTex.BlurTexture(blurSize, blurAmount);
					}
					terrainMesh.CreateFromHeightMap(&randomWalkTex, walkGridSize, walkGridSize);
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		else
		{
			if (ImGui::Button("Clear Objects"))
			{
				for (GameObject* b : boxes)
				{
					delete b;
				}
				boxes.clear();
			}
			ImGui::LabelText("Object seed", std::to_string(populateSeed).c_str());
		}

		ImGui::End();

		ImGui::Render();
		//==========================================================================

		app->Draw();

		if (selectedVar && boxes.size() == 0)
		{
			shaderBuffer.Use();
			previewPlane.Draw();
		}
		
		// END OF FRAME
		//==========================================================================
		glfwSwapBuffers(app->GetWindow()); // Displays buffer we just wrote to 
		glfwPollEvents(); // Check for inputs
		//==========================================================================
	}

	for (GameObject* b : boxes)
	{
		delete b;
	}

	for (Variant* v : trees.objectVariants)
	{
		delete v;
	}

	glfwTerminate();
	return 0;
}