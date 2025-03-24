#include "Camera.h"
#include "ext/matrix_transform.hpp"

Camera::Camera()
	: m_pos(), m_xRot(0), m_yRot(0)
{
}

Camera::Camera(glm::vec3 pos)
	: m_pos(pos), m_xRot(0), m_yRot(0)
{
}

void Camera::Update(GLFWwindow* window, float delta)
{
	glm::vec3 movement(0, 0, 0);

	if (glfwGetKey(window, GLFW_KEY_W))
	{
		movement.z += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		movement.z -= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		movement.x -= 1;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		movement.x += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		movement.y += 1;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		movement.y -= 1;
	}

	if (glfwGetKey(window, GLFW_KEY_UP))
	{
		m_yRot += delta * m_turnSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN))
	{
		m_yRot -= delta * m_turnSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT))
	{
		m_xRot -= delta * m_turnSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
	{
		m_xRot += delta * m_turnSpeed;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		// Need to get mouse pos
	}

	float magnitude = glm::length(movement);
	if (magnitude > 1)
	{
		movement /= magnitude;
	}
	//movement = glm::normalize(movement);
	glm::vec3 translatedMovement = (GetForward() * movement.z) + (GetRight() * movement.x) + (GetUp() * movement.y);
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
	return glm::lookAt(m_pos, m_pos + GetForward(), glm::vec3(0.0f, 1.0f, 0.0f));;
}
