#include "PointLight.h"
#include <iostream>

PointLight::PointLight()
	: m_col(), m_bright()
{
}

PointLight::PointLight(Mesh* mesh, Material* material)
	: GameObject(mesh, material), m_col(), m_bright(1)
{
}

PointLight::PointLight(glm::vec3 position, glm::vec3 colour, float brightness)
	: m_col(colour), m_bright(brightness)
{
	m_pos = position;
}


void PointLight::SetColour(glm::vec3 colour, float brightness)
{
	m_col = colour;
	m_bright = brightness;
}

glm::vec3 PointLight::GetColour()
{
    return m_col * m_bright;
}


void PointLight::Draw()
{
	if (!m_mesh)
	{
		std::cout << "OBJECT DOES NOT HAVE MESH" << std::endl;
		return;
	}

	m_mat->m_shader->BindUniform("modelMat", GetObjectSpace());
	m_mat->m_shader->BindUniform("colour", GetColour());

	m_mesh->Draw();
}
