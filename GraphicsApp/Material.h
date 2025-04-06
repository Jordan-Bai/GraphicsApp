#pragma once
#include "ShaderProgram.h"
#include "Texture.h"
#include <vector>

class Material
{
public:
	ShaderProgram* m_shader;

	Texture* m_albedoMap;
	Texture* m_specularMap;
	Texture* m_normalMap;

	// Material properties
	float m_ambient = 0.1f;
	float m_diffuse = 1.0f;
	float m_specular = 1.0f;

	Material(ShaderProgram* shader);
	Material(ShaderProgram* shader, Texture* albedo, Texture* specular, Texture* normal);

	void ApplyMaterial();
};