#pragma once
#include "Application.h"
#include "Mesh.h"
#include "Material.h"

class GameObject
{
protected:
	Application* m_app;

public:
	glm::vec3 m_pos;
	Mesh* m_mesh = nullptr;
	Material* m_mat = nullptr;

	GameObject();
	void Init(Application* app);

	glm::mat4 GetObjectSpace();

	virtual void Update(float delta);
	virtual void Draw();
	virtual void Draw(glm::vec3 lightDir, float specPower, glm::mat4 vpMat, glm::vec3 camPos);
};