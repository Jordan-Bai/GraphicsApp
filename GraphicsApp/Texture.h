#pragma once
#include "GraphicsHeaders.h"
#include "glm.hpp"
#include <string>
#include <vector>

struct Texture
{
	GLuint m_texture;

	Texture(std::string fileName);
	Texture(glm::vec3 colour);
	Texture(std::vector<glm::vec3> colourGrid, glm::vec2 size);
	~Texture();
};