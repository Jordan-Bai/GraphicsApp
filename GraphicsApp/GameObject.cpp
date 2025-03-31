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
	glm::mat4 translation = glm::translate(glm::mat4(1), m_pos);
	glm::mat4 rotation = glm::rotate(glm::mat4(1), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f)); // FOR TESTING
	glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(1, 1, 1)); // FOR TESTING
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

		m_mat->m_shader->SetMatrix4Uniform("modelMat", GetObjectSpace());
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
		m_mat->m_shader->SetVector3Uniform("sunDirection", lightDir);
		m_mat->m_shader->SetFloatUniform("specPower", specPower);
		m_mat->m_shader->SetMatrix4Uniform("vpMat", vpMat);
		m_mat->m_shader->SetVector3Uniform("cameraPos", camPos);
		m_mat->m_shader->SetMatrix4Uniform("modelMat", GetObjectSpace());
		m_mat->ApplyMaterial();

		m_mesh->Draw();
	}
}
