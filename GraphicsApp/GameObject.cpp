#include "GameObject.h"
#include "ext/matrix_transform.hpp"
#include <iostream>

GameObject::GameObject()
	: m_mesh(nullptr), m_mat(nullptr), m_pos(), m_rot(), m_scale({1, 1, 1})
{
	Application::Get()->AddObject(this);
}

GameObject::GameObject(Mesh* mesh, Material* material)
	: m_mesh(mesh), m_mat(material), m_pos(), m_rot(), m_scale({ 1, 1, 1 })
{
	Application::Get()->AddObject(this);
}


glm::mat4 GameObject::GetRotation()
{
	glm::mat4 xRot = glm::rotate(glm::mat4(1), m_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 yRot = glm::rotate(glm::mat4(1), m_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 zRot = glm::rotate(glm::mat4(1), m_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
	return zRot * yRot * xRot; // xRot applied first, then yRot, then zRot
}

glm::mat4 GameObject::GetObjectSpace()
{
	glm::mat4 translation = glm::translate(glm::mat4(1), m_pos);
	glm::mat4 rotation = GetRotation();
	glm::mat4 scale = glm::scale(glm::mat4(1), m_scale);
	return translation * scale * rotation;
	// ^ Applied right to left, because of the way they're being multiplied 
	// (openGL uses column-major order for matricies)
	// So rotation is applied first, then scale, then translation
}


void GameObject::Update(float delta)
{
}

void GameObject::Draw()
{
	if (!m_mesh)
	{
		std::cout << "ERROR(GameObject::Draw): Cannot draw object without mesh\n";
		return;
	}

	m_mat->m_shader->BindUniform("modelMat", GetObjectSpace());
	m_mat->ApplyMaterial();

	m_mesh->Draw();
}
