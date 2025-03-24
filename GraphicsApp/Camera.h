#pragma once
#include "GraphicsHeaders.h"
#include "glm.hpp"

class Camera
{
public:
	glm::vec3 m_pos;
	float m_moveSpeed = 3;
	float m_turnSpeed = 0.5f;
	float m_xRot;
	float m_yRot;

public:
	Camera();
	Camera(glm::vec3 pos);

	void Update(GLFWwindow* window, float delta);
	glm::vec3 GetPos();
	glm::vec3 GetForward();
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	glm::mat4 GetViewMatrix();
};