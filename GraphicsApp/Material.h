#pragma once
#include "ShaderProgram.h"
#include "Texture.h"
#include "Utilities.h"

class Material
{
public:
	ShaderProgram* m_shader;

	Texture* m_albedoMap;
	Texture* m_specularMap;
	Texture* m_normalMap;

	UniformStorage m_uniforms;

	Material(ShaderProgram* shader);
	Material(ShaderProgram* shader, Texture* albedo, Texture* specular, Texture* normal);

	void SetLightProperties(float ambient, float diffuse, float specular);

	void ApplyMaterial();
};