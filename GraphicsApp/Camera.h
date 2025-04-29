#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
	float m_moveSpeed = 3;
	float m_turnSpeed = 0.5f;
	float m_sensitivity = 60;
	float m_xRot;
	float m_yRot;

public:
	Camera();
	Camera(glm::vec3 pos);

	virtual void Update(float delta) override;
	glm::vec3 GetPos();
	glm::vec3 GetForward();
	glm::vec3 GetRight();
	glm::vec3 GetUp();
	glm::mat4 GetViewMatrix();
};