#include "Application.h"
#include "Camera.h"
#include <iostream>
#include "ext/matrix_clip_space.hpp"

Application::~Application()
{
	for (GameObject* o : m_gameObjects)
	{
		delete o;
	}
	//std::multimap<ShaderProgram*, GameObject*>::iterator it;
	//for (it = m_renderedObjects.begin(); it != m_renderedObjects.end(); it++)
	//{
	//	delete (*it).second;
	//}

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

glm::mat4 Application::GetVPMatrix()
{
	if (m_currentCamera == nullptr)
	{
		std::cout << "ERROR: No valid camera set" << std::endl;
		return glm::mat4(1);
	}

	glm::mat4 view = m_currentCamera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(
		3.14159f / 4.0f,					// FOV
		(float)m_width / (float)m_height,	// Aspect ratio
		0.3f,								// Near plane
		1000.0f);							// Far plane

	return projection * view;
	// ^ Actually applied right to left, because of the way they're being multiplied 
	// (openGL uses column-major order for matricies)
}


void Application::Update(float delta)
{
    m_lastMousePos = m_mousePos;
    double xPos;
    double yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);
    m_mousePos = {xPos, yPos};

	for (GameObject* o : m_gameObjects)
	{
		o->Update(delta);
	}
	//std::multimap<ShaderProgram*, GameObject*>::iterator it;
	//for (it = m_renderedObjects.begin(); it != m_renderedObjects.end(); it++)
	//{
	//	(*it).second->Update(delta);
	//}
}

void Application::Draw(glm::vec3 lightDir, float specPower)
{
	glm::mat4 vpMat = GetVPMatrix();
	//for (GameObject* o : m_gameObjects)
	//{
	//	o->Draw(lightDir, specPower, vpMat, m_currentCamera->GetPos());
	//}
	std::multimap<ShaderProgram*, GameObject*>::iterator it;
	ShaderProgram* currentShader = nullptr;
	for (it = m_renderedObjects.begin(); it != m_renderedObjects.end(); it++)
	{
		if ((*it).first != currentShader)
		{
			currentShader = (*it).first;
			currentShader->Use();
			currentShader->ApplyUniforms();
		}
		(*it).second->Draw();
	}
}
