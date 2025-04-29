#include "Camera.h"
#include "ext/matrix_transform.hpp"

Camera::Camera()
	: m_xRot(0), m_yRot(0)
{
}

Camera::Camera(glm::vec3 pos)
	: m_xRot(0), m_yRot(0)
{
	m_pos = pos;
}

void Camera::Update(float delta)
{
	glm::vec3 movement(0, 0, 0);

	if (Application::Get()->GetKeyDown(GLFW_KEY_W))
	{
		movement.z += 1;
	}
	if (Application::Get()->GetKeyDown(GLFW_KEY_S))
	{
		movement.z -= 1;
	}
	if (Application::Get()->GetKeyDown(GLFW_KEY_A))
	{
		movement.x -= 1;
	}
	if (Application::Get()->GetKeyDown(GLFW_KEY_D))
	{
		movement.x += 1;
	}
	if (Application::Get()->GetKeyDown(GLFW_KEY_SPACE))
	{
		movement.y += 1;
	}
	if (Application::Get()->GetKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		movement.y -= 1;
	}

	if (Application::Get()->GetKeyDown(GLFW_KEY_UP))
	{
		m_yRot += delta * m_turnSpeed;
	}
	if (Application::Get()->GetKeyDown(GLFW_KEY_DOWN))
	{
		m_yRot -= delta * m_turnSpeed;
	}
	if (Application::Get()->GetKeyDown(GLFW_KEY_LEFT))
	{
		m_xRot -= delta * m_turnSpeed;
	}
	if (Application::Get()->GetKeyDown(GLFW_KEY_RIGHT))
	{
		m_xRot += delta * m_turnSpeed;
	}

	if (Application::Get()->GetMouseButtonDown(GLFW_MOUSE_BUTTON_2))
	{
		glm::vec2 mouseMove = Application::Get()->GetMouseDelta() * (m_sensitivity / 50000);
		m_xRot -= mouseMove.x;
		m_yRot += mouseMove.y;
	}

	// Y movement isn't relative to rotation, because it just feels nicer that way
	glm::vec3 horizontalMovement = (GetForward() * movement.z) + (GetRight() * movement.x);
	glm::vec3 translatedMovement = { horizontalMovement.x, movement.y, horizontalMovement.z };

	float magnitude = glm::length(translatedMovement);
	// Normalize the movement, but only if it's longer than 0, to prevent dividing by 0
	if (magnitude > 0.01f)
	{
		translatedMovement /= magnitude;
	}
	m_pos += translatedMovement * delta * m_moveSpeed;
}

glm::vec3 Camera::GetPos()
{
	return m_pos;
}

glm::vec3 Camera::GetForward()
{
	return glm::vec3(sin(m_xRot) * cos(m_yRot), sin(m_yRot), -cos(m_xRot) * cos(m_yRot));
}

glm::vec3 Camera::GetRight()
{
	return glm::vec3(cos(m_xRot) * cos(m_yRot), sin(m_yRot), sin(m_xRot) * cos(m_yRot));
}

glm::vec3 Camera::GetUp()
{
	return glm::vec3(sin(m_xRot) * sin(m_yRot), cos(m_yRot), cos(m_xRot) * sin(m_yRot));
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_pos, m_pos + GetForward(), glm::vec3(0.0f, 1.0f, 0.0f));
}
