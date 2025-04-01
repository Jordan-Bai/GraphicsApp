#include "Shader.h"
#include "Utilities.h"
#include <iostream>

Shader::Shader(std::string filename, GLuint shaderType)
{
	if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER)
	{
		std::cout << "ERROR: Invalid shader type" << std::endl;
	}

	m_fileName = filename;
	m_type = shaderType;

	m_shaderID = glCreateShader(m_type);

	bool foundProblem = false;

	std::string shaderSource = LoadFileAsString(filename);
	if (shaderSource == "")
	{
		std::cout << "Failed to open one the shader source file.\n";
		std::cout << "Is your working directory set up correctly?\n";
		foundProblem = true;
	}
	else
	{
		const char* shaderSourceC = shaderSource.c_str();

		glShaderSource(m_shaderID, 1, &shaderSourceC, nullptr);
		glCompileShader(m_shaderID);

		std::string shaderName;
		if (m_type == GL_VERTEX_SHADER)
		{
			shaderName = "Vertex shader \"";
		}
		else
		{
			shaderName = "Fragment shader \"";
		}
		shaderName += filename + "\"";

		GLchar errorLog[512];
		GLint success = 0;
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			std::cout << "Something went wrong with " << shaderName << "!\n";
			glGetShaderInfoLog(m_shaderID, 512, nullptr, errorLog);
			std::cout << errorLog << '\n';
			foundProblem = true;
		}
		else
		{
			std::cout << shaderName << " loaded successfuly!\n";
		}
	}

	if (foundProblem)
	{
		glDeleteShader(m_shaderID);
		m_shaderID = 0;
	}
}

Shader::~Shader()
{
	glDeleteShader(m_shaderID);
}

bool Shader::ShaderLoaded()
{
	return m_shaderID != 0;
}

void Shader::Reload()
{
	glDeleteShader(m_shaderID);

	m_shaderID = glCreateShader(m_type);

	bool foundProblem = false;

	std::string shaderSource = LoadFileAsString(m_fileName);
	if (shaderSource == "")
	{
		std::cout << "Failed to open one the shader source file.\n";
		std::cout << "Is your working directory set up correctly?\n";
		foundProblem = true;
	}
	else
	{
		const char* shaderSourceC = shaderSource.c_str();

		glShaderSource(m_shaderID, 1, &shaderSourceC, nullptr);
		glCompileShader(m_shaderID);

		std::string shaderName;
		if (m_type == GL_VERTEX_SHADER)
		{
			shaderName = "Vertex shader \"";
		}
		else
		{
			shaderName = "Fragment shader \"";
		}
		shaderName += m_fileName + "\"";

		GLchar errorLog[512];
		GLint success = 0;
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			std::cout << "Something went wrong with " << shaderName << "!\n";
			glGetShaderInfoLog(m_shaderID, 512, nullptr, errorLog);
			std::cout << errorLog << '\n';
			foundProblem = true;
		}
		else
		{
			std::cout << shaderName << " loaded successfuly!\n";
		}
	}

	if (foundProblem)
	{
		glDeleteShader(m_shaderID);
		m_shaderID = 0;
	}
}

GLuint Shader::GetShaderID()
{
	return m_shaderID;
}
