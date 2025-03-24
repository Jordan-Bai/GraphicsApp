#include "Application.h"
#include <iostream>
#include "ext/matrix_clip_space.hpp"

Application::~Application()
{
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

glm::mat4 Application::GetProjection()
{
	return glm::perspective(
		3.14159f / 4.0f,					// FOV
		(float)m_width / (float)m_height,	// Aspect ratio
		0.3f,								// Near plane
		1000.0f);							// Far plane
}

void Application::Update(float delta)
{
    m_lastMousePos = m_mousePos;
    double xPos;
    double yPos;
    glfwGetCursorPos(m_window, &xPos, &yPos);
    m_mousePos = {xPos, yPos};
}
