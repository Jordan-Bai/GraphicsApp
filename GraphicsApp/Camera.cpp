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

	if (glfwGetKey(Application::Get()->GetWindow(), GLFW_KEY_W))
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
		// Need to get mouse pos
	}

	float magnitude = glm::length(movement);
	if (magnitude > 1)
	{
		movement /= magnitude;
	}
	//movement = glm::normalize(movement);
	glm::vec3 translatedMovement = (GetForward() * movement.z) + (GetRight() * movement.x) + glm::vec3{0, movement.y, 0};
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
	//glm::vec3 forward(sin(m_xRot) * cos(m_yRot), sin(m_yRot), cos(m_xRot) * cos(m_yRot));

	return glm::lookAt(m_pos, m_pos + GetForward(), glm::vec3(0.0f, 1.0f, 0.0f));
	//return glm::lookAt(m_pos - (GetForward() * zoom), m_pos, glm::vec3(0.0f, 1.0f, 0.0f));
	// If you want the camera pos to be the point it's focused on

	//return glm::translate(glm::mat4(1), -m_pos);
}
