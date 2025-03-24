#pragma once
#include "ShaderProgram.h"
#include "Texture.h"
#include <vector>

class Material
{
public:
	ShaderProgram* m_shader;

	Texture* m_albedo;
	Texture* m_specular;
	Texture* m_normal;

	Material(ShaderProgram* shader);

	void ApplyMaterial();
};