#pragma once

#include "GraphicsHeaders.h"
#include <string>

class Shader
{
	GLuint m_shaderID = 0;
	GLuint m_type = 0; // Vertex or fragment
	std::string m_fileName;

public:
	Shader(std::string filename, GLuint shaderType);
	~Shader();

	bool ShaderLoaded();
	void Reload();
	GLuint GetShaderID();
};