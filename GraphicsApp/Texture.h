#pragma once
#include "GraphicsHeaders.h"
#include "glm.hpp"
#include <string>

struct Texture
{
	GLuint m_texture;

	~Texture();
	void LoadFileAsTexture(std::string fileName);
	void CreateColourTexture(glm::vec3 colour);
};