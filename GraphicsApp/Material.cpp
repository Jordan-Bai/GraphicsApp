#include "Material.h"
#include <iostream>

Material::Material(ShaderProgram* shader)
	: m_shader(shader)
{
}

Material::Material(ShaderProgram* shader, Texture* albedo, Texture* specular, Texture* normal)
	: m_shader(shader), m_albedoMap(albedo), m_specularMap(specular), m_normalMap(normal)
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
	if (m_albedoMap)
	{
		glBindTexture(GL_TEXTURE_2D, m_albedoMap->m_texture);
	}
	glActiveTexture(GL_TEXTURE1);
	if (m_specularMap)
	{
		glBindTexture(GL_TEXTURE_2D, m_specularMap->m_texture);
	}
	glActiveTexture(GL_TEXTURE2);
	if (m_normalMap)
	{
		glBindTexture(GL_TEXTURE_2D, m_normalMap->m_texture);
	}

	m_shader->BindUniform("albedoMap", 0);
	m_shader->BindUniform("specularMap", 1);
	m_shader->BindUniform("normalMap", 2);

	m_shader->BindUniform("matAmbient", m_ambient);
	m_shader->BindUniform("matDiffuse", m_diffuse);
	m_shader->BindUniform("matSpecular", m_specular);
}
