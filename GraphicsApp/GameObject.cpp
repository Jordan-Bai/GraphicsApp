#include "GameObject.h"
#include "ext/matrix_transform.hpp"
#include <iostream>

GameObject::GameObject()
	: m_mesh(nullptr), m_mat(nullptr), m_pos(), m_rot(), m_scale({1, 1, 1})
{
}

GameObject::GameObject(Mesh* mesh, Material* material)
	: m_mesh(mesh), m_mat(material), m_pos(), m_rot(), m_scale({ 1, 1, 1 })
{
}

void GameObject::Init(Application* app)
{
	m_app = app;
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
	// ^ Actually applied right to left, because of the way they're being multiplied 
	// (openGL uses column-major order for matricies)
	// So rotation is applied first, then scale, then translation
}

void GameObject::Update(float delta)
{
}

void GameObject::Draw()
{
	if (m_mesh)
	{
		if (!m_mat)
		{
			std::cout << "MESH DOES NOT HAVE MATERIAL" << std::endl;
			return;
		}

		m_mat->m_shader->BindUniform("modelMat", GetObjectSpace());
		m_mat->ApplyMaterial();

		m_mesh->Draw();
	}
}

void GameObject::Draw(glm::vec3 lightDir, float specPower, glm::mat4 vpMat, glm::vec3 camPos)
{
	if (m_mesh)
	{
		if (!m_mat)
		{
			std::cout << "MESH DOES NOT HAVE MATERIAL" << std::endl;
			return;
		}

		m_mat->m_shader->Use();
		m_mat->m_shader->BindUniform("sunDirection", lightDir);
		m_mat->m_shader->BindUniform("specPower", specPower);
		m_mat->m_shader->BindUniform("vpMat", vpMat);
		m_mat->m_shader->BindUniform("cameraPos", camPos);
		m_mat->m_shader->BindUniform("modelMat", GetObjectSpace());
		m_mat->ApplyMaterial();

		m_mesh->Draw();
	}
}
