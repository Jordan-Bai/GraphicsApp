#pragma once
#include "GraphicsHeaders.h"
#include <string>

struct Texture
{
	GLuint m_texture;

	~Texture();
	void LoadFileAsTexture(std::string fileName);
};