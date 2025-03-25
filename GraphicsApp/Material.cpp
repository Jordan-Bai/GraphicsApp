#include "Material.h"
#include <iostream>

Material::Material(ShaderProgram* shader)
	: m_shader(shader)
{
}

Material::Material(ShaderProgram* shader, Texture* albedo, Texture* specular, Texture* normal)
	: m_shader(shader), m_albedo(albedo), m_specular(specular), m_normal(normal)
{
}

void Material::ApplyMaterial()
{
	if (!m_shader)
	{
		std::cout << "ERROR: Material does not contain shader reference" << std::endl;
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	if (m_albedo)
	{
		glBindTexture(GL_TEXTURE_2D, m_albedo->m_texture);
	}
	glActiveTexture(GL_TEXTURE1);
	if (m_specular)
	{
		glBindTexture(GL_TEXTURE_2D, m_specular->m_texture);
	}
	glActiveTexture(GL_TEXTURE2);
	if (m_specular)
	{
		glBindTexture(GL_TEXTURE_2D, m_normal->m_texture);
	}

	m_shader->SetIntUniform("albedoMap", 0);
	m_shader->SetIntUniform("specularMap", 1);
	m_shader->SetIntUniform("normalMap", 2);
}
