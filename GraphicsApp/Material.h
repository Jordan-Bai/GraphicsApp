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

	float m_specPower = 1;

	Material(ShaderProgram* shader);
	Material(ShaderProgram* shader, Texture* albedo, Texture* specular, Texture* normal);

	void ApplyMaterial();

	//void SetVPMat();
};