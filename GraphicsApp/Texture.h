#pragma once
#include "GraphicsHeaders.h"
#include "glm.hpp"
#include <string>

struct Texture
{
	GLuint m_texture;

	Texture(std::string fileName);
	Texture(glm::vec3 colour);
	~Texture();
};