#include "PointLight.h"

PointLight::PointLight()
	: m_col(), m_bright()
{
}

PointLight::PointLight(Mesh* mesh, Material* material)
	: GameObject(mesh, material)
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
	m_mat->m_shader->BindUniform("modelMat", GetObjectSpace());
	m_mat->m_shader->BindUniform("colour", GetColour());

	m_mesh->Draw();
}
