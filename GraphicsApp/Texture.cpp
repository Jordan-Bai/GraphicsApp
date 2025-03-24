#include "Texture.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION // Stops it from including unecessary files?
#include "stb_image.h"

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::LoadFileAsTexture(std::string fileName)
{
	glGenTextures(1, &m_texture); // Generate 1 texture buffer

	glBindTexture(GL_TEXTURE_2D, m_texture);

	int width, height, channels; // Channels stores how many colour channels we have (4 w/ alpha, 3 w/o)
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
	// 0 tells it to 'load the image as is', not really sure what that means

	//GLuint format;
	//switch (channels)
	//{
	//case 1:
	//	format = GL_RED;
	//	break;
	//case 2:
	//	format = GL_RG;
	//	break;
	//case 3:
	//	format = GL_RGB;
	//	break;
	//case 4:
	//	format = GL_RGBA;
	//	break;
	//default:
	//	std::cout << "ERROR: Invalid number of channels" << std::endl;
	//	stbi_image_free(data);
	//	return;
	//}

	if (data) // if the data was retrieved successfully
	{
		glTexImage2D(GL_TEXTURE_2D,		// The type of texture to generate
			0,							// The 'mipmap level' (0 being the highest one)
			GL_RGB,					// External format (what channel format is being uploaded)
			width, height,				// The image specifications
			0,							// Always 0 for legacy reasons apparently
			GL_RGB,						// Format (what channel format is actually used internally)
			GL_UNSIGNED_BYTE,			// Not sure about this one
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
