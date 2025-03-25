#include "GameObject.h"
#include "ext/matrix_transform.hpp"
#include <iostream>

GameObject::GameObject()
	: m_app()
{
}

void GameObject::Init(Application* app)
{
	m_app = app;
}

glm::mat4 GameObject::GetObjectSpace()
{
	return glm::translate(glm::mat4(1), m_pos);
}

void GameObject::Update(float delta)
{
}

void GameObject::Draw()
{
	if (m_mesh)
	{
		m_mat->ApplyMaterial();
		m_mat->m_shader->SetMatrix4Uniform("modelMat", GetObjectSpace());

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

		m_mat->ApplyMaterial();
		m_mat->m_shader->SetVector3Uniform("sunDirection", lightDir);
		m_mat->m_shader->SetFloatUniform("specPower", specPower);
		m_mat->m_shader->SetMatrix4Uniform("vpMat", vpMat);
		m_mat->m_shader->SetVector3Uniform("cameraPos", camPos);
		m_mat->m_shader->SetMatrix4Uniform("modelMat", GetObjectSpace());

		m_mesh->Draw();
	}
}
