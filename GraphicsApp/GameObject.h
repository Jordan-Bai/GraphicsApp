#pragma once
#include "Application.h"

class GameObject
{
protected:
	Application* m_app;

public:
	glm::vec3 m_pos;

	GameObject();
	void Init(Application* app);

	glm::mat4 GetObjectSpace();
};