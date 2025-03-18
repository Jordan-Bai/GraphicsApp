#pragma once
#include "ShaderProgram.h"
#include "Texture.h"
#include <vector>

class Material
{
	ShaderProgram* m_shader;

	std::vector<Texture> m_textures;
};