#pragma once
#include "GraphicsHeaders.h"
#include "glm.hpp"
#include <string>
#include <vector>

struct Texture
{
	GLuint m_texture;
	glm::vec2 m_size;

	Texture(std::string fileName);
	Texture(glm::vec3 colour);
	Texture(std::vector<glm::vec3> colourGrid, glm::vec2 size);
	Texture(char* colourData, glm::vec2 size, GLenum format, GLenum type);
	Texture(const Texture& other);
	~Texture();

	void BlurTexture();
	void BlurTexture(int size, float blurStrength);

	Texture& operator=(const Texture& other);
};