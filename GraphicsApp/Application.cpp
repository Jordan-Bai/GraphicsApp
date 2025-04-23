#include "Application.h"
#include "Camera.h"
#include <iostream>
#include "ext/matrix_clip_space.hpp"

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Application::Application()
	: m_window(nullptr), m_currentCamera(nullptr)
{
}

Application::~Application()
{
	//for (GameObject* o : m_gameObjects)
	//{
	//	delete o;
	//}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}


int Application::Initialize()
{
    //glfwSetCursorPosCallback(m_window, SetMousePos);

	if (!glfwInit())
	{
		std::cout << "GLFW COULD NOT INIT" << std::endl;
		return -1;
	}

	m_window = glfwCreateWindow(m_width, m_height, "Test Window", nullptr, nullptr);
	if (!m_window)
	{
		glfwTerminate();
		std::cout << "WINDOW COULD NOT BE CREATED" << std::endl;
		return -1;
	}

	// Make window the current window
	glfwMakeContextCurrent(m_window);

	if (!gladLoadGL())
	{
		glfwTerminate();
		std::cout << "GLAD COULD NOT LOAD" << std::endl;
		return -1;
	}

	// IMGUI SETUP
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init();
}

std::vector<GameObject*> Application::GetObjects()
{
	return m_gameObjects;
}


void Application::AddObject(GameObject* object)
{
	m_gameObjects.push_back(object);
	object->Init(this);
	if (object->m_mat != nullptr)
	{
		if (object->m_mat->m_shader == nullptr)
		{
			std::cout << "MATERIAL DOES NOT CONTAIN SHADER" << std::endl;
			return;
		}

		std::multimap<ShaderProgram*, GameObject*>::iterator it = m_renderedObjects.find(object->m_mat->m_shader);
		if (it == m_renderedObjects.end()) // If the shader isn't already registered, add it to the list
		{
			m_shaders.push_back(object->m_mat->m_shader);
		}
		m_renderedObjects.insert(std::pair<ShaderProgram*, GameObject*>(object->m_mat->m_shader, object));
	}
}

Camera* Application::GetCurrentCamera()
{
	return m_currentCamera;
}

void Application::SetCurrentCamera(Camera* camera)
{
	m_currentCamera = camera;
}

GLFWwindow* Application::GetWindow()
{
    return m_window;
}


void Application::BindUniformInAllShaders(std::string uniformName, int value)
{
	for (ShaderProgram* s : m_shaders)
	{
		s->Use();
		s->BindUniform(uniformName, value);
	}
}

void Application::BindUniformInAllShaders(std::string uniformName, float value)
{
	for (ShaderProgram* s : m_shaders)
	{
		s->Use();
		s->BindUniform(uniformName, value);
	}
}

void Application::BindUniformInAllShaders(std::string uniformName, glm::vec3 value)
{
	for (ShaderProgram* s : m_shaders)
	{
		s->Use();
		s->BindUniform(uniformName, value);
	}
}

void Application::BindUniformInAllShaders(std::string uniformName, glm::mat4 value)
{
	for (ShaderProgram* s : m_shaders)
	{
		s->Use();
		s->BindUniform(uniformName, value);
	}
}


void Application::SetUniformInAllShaders(std::string uniformName, int value)
{
	for (ShaderProgram* s : m_shaders)
	{
		s->m_uniforms.SetUniform(uniformName, value);
	}
}

void Application::SetUniformInAllShaders(std::string uniformName, float value)
{
	for (ShaderProgram* s : m_shaders)
	{
		s->m_uniforms.SetUniform(uniformName, value);
	}
}

void Application::SetUniformInAllShaders(std::string uniformName, glm::vec3 value)
{
	for (ShaderProgram* s : m_shaders)
	{
		s->m_uniforms.SetUniform(uniformName, value);
	}
}

void Application::SetUniformInAllShaders(std::string uniformName, glm::mat4 value)
{
	for (ShaderProgram* s : m_shaders)
	{
		s->m_uniforms.SetUniform(uniformName, value);
	}
}


void Application::ReloadShaders()
{
	for (ShaderProgram* s : m_shaders)
	{
		s->ReloadShader();
	}
}

void Application::ApplyAllUniforms()
{
	for (ShaderProgram* s : m_shaders)
	{
		s->ApplyUniforms();
	}
}


glm::vec2 Application::GetMousePos()
{
    return m_mousePos;
}

glm::vec2 Application::GetMouseDelta()
{
    return m_mousePos - m_lastMousePos;
}

bool Application::GetKeyDown(int key)
{
	return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool Application::GetMouseButtonDown(int button)
{
	return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

glm::mat4 Application::GetProjectionMatrix()
{
	return glm::perspective(
		3.14159f / 4.0f,					// FOV
		(float)m_width / (float)m_height,	// Aspect ratio
		0.3f,								// Near plane
		1000.0f);							// Far plane;
}

glm::mat4 Application::GetVPMatrix()
{
	if (m_currentCamera == nullptr)
	{
		std::cout << "ERROR: No valid camera set" << std::endl;
		return glm::mat4(1);
	}

	glm::mat4 view = m_currentCamera->GetViewMatrix();

	return GetProjectionMatrix() * view;
	// ^ Actually applied right to left, because of the way they're being multiplied 
	// (openGL uses column-major order for matricies)
}

float Application::GetAspectRatio()
{
	return (float)m_width / (float)m_height;
}


void Application::Update(float delta)
{
    m_lastMousePos = m_mousePos;
    double xPos;
    double yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);
    m_mousePos = {xPos, yPos};

	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	for (GameObject* o : m_gameObjects)
	{
		o->Update(delta);
	}

	//ImGui::Render(); // Doesn't actually draw stuff, just prepares info to be drawn
}

void Application::Draw()
{
	glm::mat4 vpMat = GetVPMatrix();
	std::multimap<ShaderProgram*, GameObject*>::iterator it;
	ShaderProgram* currentShader = nullptr;

	for (it = m_renderedObjects.begin(); it != m_renderedObjects.end(); it++)
	{
		if ((*it).first != currentShader)
		{
			currentShader = (*it).first;
			currentShader->Use();
			//currentShader->ApplyUniforms();
		}
		(*it).second->Draw();
	}

	// ImGui
	ImDrawData* drawData = ImGui::GetDrawData();
	if (drawData != nullptr)
	{
		ImGui_ImplOpenGL3_RenderDrawData(drawData);
	}
}
