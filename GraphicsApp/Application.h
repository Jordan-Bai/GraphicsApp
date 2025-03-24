#pragma once
#include "GraphicsHeaders.h"
#include "glm.hpp"

class Application
{
	GLFWwindow* m_window;

	glm::vec2 m_mousePos;
	glm::vec2 m_lastMousePos;

	float m_width = 1280;
	float m_height = 720;

public:
	~Application();

	int Initialize();

	GLFWwindow* GetWindow();
	glm::vec2 GetMousePos();
	glm::vec2 GetMouseDelta();

	glm::mat4 GetProjection();

	void Update(float delta);
};