#include "Application.h"
#include "Camera.h"
#include <iostream>
#include "ext/matrix_clip_space.hpp"

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Application* Application::s_instance;

Application::Application()
	: m_window(nullptr), m_currentCamera(nullptr), m_mousePos(), m_lastMousePos()
{
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}


Application* Application::Get()
{
	if (s_instance == nullptr)
	{
		s_instance = new Application();
	}
	return s_instance;
}

int Application::Initialize()
{
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

	glEnable(GL_DEPTH_TEST); // Enables depth buffer

	// Input setup
	glfwSetKeyCallback(m_window, &Application::KeyCallback);
	glfwSetCursorPosCallback(m_window, &Application::CursorPosCallback);

	// ImGui setup
	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init();
}


void Application::AddObject(GameObject* object)
{
	if (object->m_mat != nullptr && object->m_mat->m_shader != nullptr)
	{
		m_gameObjects.insert(std::pair<ShaderProgram*, GameObject*>(object->m_mat->m_shader, object));
	}
	else
	{
		m_gameObjects.insert(std::pair<ShaderProgram*, GameObject*>(nullptr, object));
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
	std::multimap<ShaderProgram*, GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (it->first != nullptr)
		{
			it->first->Use();
			it->first->BindUniform(uniformName, value);
		}
		it = m_gameObjects.upper_bound(it->first);
	}
}

void Application::BindUniformInAllShaders(std::string uniformName, float value)
{
	std::multimap<ShaderProgram*, GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (it->first != nullptr)
		{
			it->first->Use();
			it->first->BindUniform(uniformName, value);
		}
		it = m_gameObjects.upper_bound(it->first);
	}
}

void Application::BindUniformInAllShaders(std::string uniformName, glm::vec3 value)
{
	std::multimap<ShaderProgram*, GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (it->first != nullptr)
		{
			it->first->Use();
			it->first->BindUniform(uniformName, value);
		}
		it = m_gameObjects.upper_bound(it->first);
	}
}

void Application::BindUniformInAllShaders(std::string uniformName, glm::mat4 value)
{
	std::multimap<ShaderProgram*, GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (it->first != nullptr)
		{
			it->first->Use();
			it->first->BindUniform(uniformName, value);
		}
		it = m_gameObjects.upper_bound(it->first);
	}
}


void Application::SetUniformInAllShaders(std::string uniformName, int value)
{
	std::multimap<ShaderProgram*, GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (it->first != nullptr)
		{
			it->first->m_uniforms.SetUniform(uniformName, value);
		}
		it = m_gameObjects.upper_bound(it->first);
	}
}

void Application::SetUniformInAllShaders(std::string uniformName, float value)
{
	std::multimap<ShaderProgram*, GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (it->first != nullptr)
		{
			it->first->m_uniforms.SetUniform(uniformName, value);
		}
		it = m_gameObjects.upper_bound(it->first);
	}
}

void Application::SetUniformInAllShaders(std::string uniformName, glm::vec3 value)
{
	std::multimap<ShaderProgram*, GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (it->first != nullptr)
		{
			it->first->m_uniforms.SetUniform(uniformName, value);
		}
		it = m_gameObjects.upper_bound(it->first);
	}
}

void Application::SetUniformInAllShaders(std::string uniformName, glm::mat4 value)
{
	std::multimap<ShaderProgram*, GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (it->first != nullptr)
		{
			it->first->m_uniforms.SetUniform(uniformName, value);
		}
		it = m_gameObjects.upper_bound(it->first);
	}
}


void Application::ReloadShaders()
{
	std::cout << "=====RELOADING ALL SHADERS=====" << std::endl;
	std::multimap<ShaderProgram*, GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (it->first != nullptr)
		{
			it->first->ReloadShader();
		}
		it = m_gameObjects.upper_bound(it->first);
	}
	std::cout << "=====ALL SHADERS RELOADED=====" << std::endl;
}

void Application::ApplyAllUniforms()
{
	std::multimap<ShaderProgram*, GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if (it->first != nullptr)
		{
			it->first->ApplyUniforms();
		}
		it = m_gameObjects.upper_bound(it->first);
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


void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		s_instance->ReloadShaders();
	}
}

void Application::CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	s_instance->m_mousePos = { xPos, yPos };
}

void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
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
	// ^ Applied right to left, because of the way they're being multiplied 
	// (openGL uses column-major order for matricies)
}

float Application::GetAspectRatio()
{
	return (float)m_width / (float)m_height;
}


void Application::Update(float delta)
{
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	for (std::pair<ShaderProgram*, GameObject*> p : m_gameObjects)
	{
		p.second->Update(delta);
	}

	m_lastMousePos = m_mousePos;
}

void Application::Draw()
{
	glm::mat4 vpMat = GetVPMatrix();
	std::multimap<ShaderProgram*, GameObject*>::iterator it;
	ShaderProgram* currentShader = nullptr;

	for (it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		if ((*it).first == nullptr)
		{
			continue;
		}
		if ((*it).first != currentShader)
		{
			currentShader = (*it).first;
			currentShader->Use();
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
