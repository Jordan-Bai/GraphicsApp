#include "Texture.h"
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION // Stops it from including unecessary files?
#include "stb_image.h"

Texture::Texture(std::string fileName)
{
	glGenTextures(1, &m_texture); // Generate 1 texture buffer

	glBindTexture(GL_TEXTURE_2D, m_texture);

	int width, height, channels; // Channels stores how many colour channels we have (4 w/ alpha, 3 w/o)
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
	// 0 tells it to 'load the image as is', not really sure what that means

	GLenum format;
	switch (channels)
	{
	case 1:
		format = GL_RED;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}

	m_size = { width, height };

	if (data) // if the data was retrieved successfully
	{
		glTexImage2D(GL_TEXTURE_2D,		// The type of texture to generate
			0,							// The 'mipmap level' (0 being the highest one)
			GL_RGB,						// Internal format (what channel format is used internally)
			width, height,				// The image specifications
			0,							// Always 0 for legacy reasons apparently
			format,						// Format (what channel format is being uploaded)
			GL_UNSIGNED_BYTE,			// The type of the data
			data);						// The data itself

		glGenerateMipmap(GL_TEXTURE_2D); // Mip-mapping
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data); // Free the data, good practice for not causing memory leaks
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(glm::vec3 colour)
{
	m_size = {2, 2};
	std::vector<glm::vec3> colourGrid = { colour, colour, colour, colour };
	glGenTextures(1, &m_texture); // Generate 1 texture buffer

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D,		// The type of texture to generate
		0,							// The 'mipmap level' (0 being the highest one)
		GL_RGB,						// Internal format (what channel format is used internally)
		2, 2,						// The image specifications
		0,							// Always 0 for legacy reasons apparently
		GL_RGB,						// Format (what channel format is being uploaded)
		GL_FLOAT,					// The type of the data
		(char*)colourGrid.data());	// The data itself

	glGenerateMipmap(GL_TEXTURE_2D); // Mip-mapping
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(std::vector<glm::vec3> colourGrid, glm::vec2 size)
{
	m_size = size;
	glGenTextures(1, &m_texture); // Generate 1 texture buffer

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D,		// The type of texture to generate
		0,							// The 'mipmap level' (0 being the highest one)
		GL_RGB,						// Internal format (what channel format is used internally)
		size.x, size.y,				// The image specifications
		0,							// Always 0 for legacy reasons apparently
		GL_RGB,						// Format (what channel format is being uploaded)
		GL_FLOAT,					// The type of the data
		(char*)colourGrid.data());	// The data itself

	glGenerateMipmap(GL_TEXTURE_2D); // Mip-mapping
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(char* colourData, glm::vec2 size, GLenum format, GLenum type)
{
	m_size = size;
	glGenTextures(1, &m_texture); // Generate 1 texture buffer

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D,		// The type of texture to generate
		0,							// The 'mipmap level' (0 being the highest one)
		GL_RGB,						// Internal format (what channel format is used internally)
		size.x, size.y,				// The image specifications
		0,							// Always 0 for legacy reasons apparently
		format,						// Format (what channel format is being uploaded)
		type,						// The type of the data
		colourData);				// The data itself

	glGenerateMipmap(GL_TEXTURE_2D); // Mip-mapping
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const Texture& other)
	: m_size(other.m_size)
{
	std::vector<glm::vec3> pixels(m_size.x * m_size.y);
	glBindTexture(GL_TEXTURE_2D, other.m_texture);
	glGetTexImage(GL_TEXTURE_2D,		// The type of texture to generate
		0,								// The 'mipmap level' (0 being the highest one)
		GL_RGB,							// Internal format (what channel format is used internally)
		GL_FLOAT,						// The type of the data
		pixels.data());

	glGenTextures(1, &m_texture); // Generate 1 texture buffer

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D,		// The type of texture to generate
		0,							// The 'mipmap level' (0 being the highest one)
		GL_RGB,						// Internal format (what channel format is used internally)
		2, 2,						// The image specifications
		0,							// Always 0 for legacy reasons apparently
		GL_RGB,						// Format (what channel format is being uploaded)
		GL_FLOAT,					// The type of the data
		pixels.data());				// The data itself

	glGenerateMipmap(GL_TEXTURE_2D); // Mip-mapping
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::BlurTexture()
{
	BlurTexture(1, 0.5f);
}

void Texture::BlurTexture(int size, float blurStrength)
{
	std::vector<glm::vec3> pixels(m_size.x * m_size.y);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glGetTexImage(GL_TEXTURE_2D,		// The type of texture to generate
		0,								// The 'mipmap level' (0 being the highest one)
		GL_RGB,							// Internal format (what channel format is used internally)
		GL_FLOAT,						// The type of the data
		pixels.data());

	std::vector<glm::vec3> newPixels(m_size.x * m_size.y);

	for (int x = 0; x < m_size.x; x++)
	{
		for (int y = 0; y < m_size.y; y++)
		{
			int thisTile = x + (y * m_size.x);

			int tileCount;
			glm::vec3 adjacentColours(0);

			int minX = x;
			int maxX = x;
			int minY = y;
			int maxY = y;
			for (int i = 0; i < size; i++)
			{
				if (minX > 0)
				{
					minX--;
				}
				if (maxX < m_size.x - 1)
				{
					maxX++;
				}

				if (minY > 0)
				{
					minY--;
				}
				if (maxY < m_size.y - 1)
				{
					maxY++;
				}
			}

			tileCount = (maxX - minX + 1) * (maxY - minY + 1);

			for (int x2 = minX; x2 <= maxX; x2++)
			{
				for (int y2 = minY; y2 <= maxY; y2++)
				{
					adjacentColours += pixels[x2 + (y2 * m_size.x)];
				}
			}

			adjacentColours = adjacentColours / (float)tileCount;
			newPixels[thisTile] = (pixels[thisTile] * (1.0f - blurStrength)) + (adjacentColours * blurStrength);
		}
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D,		// The type of texture to generate
		0,							// The 'mipmap level' (0 being the highest one)
		GL_RGB,						// Internal format (what channel format is used internally)
		m_size.x, m_size.y,			// The image specifications
		0,							// Always 0 for legacy reasons apparently
		GL_RGB,						// Format (what channel format is being uploaded)
		GL_FLOAT,					// The type of the data
		newPixels.data());			// The data itself

	glGenerateMipmap(GL_TEXTURE_2D); // Mip-mapping
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture& Texture::operator=(const Texture& other)
{
	std::vector<glm::vec3> pixels(m_size.x * m_size.y);
	glBindTexture(GL_TEXTURE_2D, other.m_texture);
	glGetTexImage(GL_TEXTURE_2D,		// The type of texture to generate
		0,								// The 'mipmap level' (0 being the highest one)
		GL_RGB,							// Internal format (what channel format is used internally)
		GL_FLOAT,						// The type of the data
		pixels.data());
	
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D,		// The type of texture to generate
		0,							// The 'mipmap level' (0 being the highest one)
		GL_RGB,						// Internal format (what channel format is used internally)
		2, 2,						// The image specifications
		0,							// Always 0 for legacy reasons apparently
		GL_RGB,						// Format (what channel format is being uploaded)
		GL_FLOAT,					// The type of the data
		pixels.data());				// The data itself

	glGenerateMipmap(GL_TEXTURE_2D); // Mip-mapping
	glBindTexture(GL_TEXTURE_2D, 0);

	return *this;
}
