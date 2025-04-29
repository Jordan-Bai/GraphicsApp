#pragma once
#include "Application.h"
#include "Mesh.h"
#include "Material.h"

class GameObject
{
public:
	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
	Mesh* m_mesh;
	Material* m_mat;

	GameObject();
	GameObject(Mesh* mesh, Material* material);

	glm::mat4 GetRotation();
	glm::mat4 GetObjectSpace();

	virtual void Update(float delta);
	virtual void Draw();
};