#include "ShaderProgram.h"
#include "Utilities.h"
#include <iostream>

ShaderProgram::ShaderProgram(Shader* vertexShader, Shader* fragmentShader)
{
	if (vertexShader == nullptr || fragmentShader == nullptr)
	{
		std::cout << "ERROR: One or more of the shaders provided was null.\n";
	}
	m_vertShader = vertexShader;
	m_fragShader = fragmentShader;

	shaderProgram = glCreateProgram();

	bool foundProblem = false;

	if (m_vertShader->ShaderLoaded() && m_fragShader->ShaderLoaded())
	{
		glAttachShader(shaderProgram, m_vertShader->GetShaderID());
		glAttachShader(shaderProgram, m_fragShader->GetShaderID());

		glLinkProgram(shaderProgram);

		GLchar errorLog[512];
		GLint success = 0;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cout << "Error linking the shaders.\n";
			glGetProgramInfoLog(shaderProgram, 512, nullptr, errorLog);
			std::cout << errorLog << '\n';
			foundProblem = true;
		}
		else
		{
			std::cout << "The shaders linked properly\n";
		}
	}
	else
	{
		std::cout << "ERROR: One or more of the shaders was invalid.\n";
	}

	if (foundProblem)
	{
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}
}

ShaderProgram::ShaderProgram(std::string vertexFilename, std::string fragmentFilename)
{
	shaderProgram = glCreateProgram();

	bool foundProblem = false;

	m_vertShader = new Shader(vertexFilename, GL_VERTEX_SHADER);
	m_fragShader = new Shader(fragmentFilename, GL_FRAGMENT_SHADER);

	if (m_vertShader->ShaderLoaded() && m_fragShader->ShaderLoaded())
	{
		glAttachShader(shaderProgram, m_vertShader->GetShaderID());
		glAttachShader(shaderProgram, m_fragShader->GetShaderID());

		glLinkProgram(shaderProgram);

		GLchar errorLog[512];
		GLint success = 0;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cout << "Error linking the shaders.\n";
			glGetProgramInfoLog(shaderProgram, 512, nullptr, errorLog);
			std::cout << errorLog << '\n';
			foundProblem = true;
		}
		else
		{
			std::cout << "The shaders linked properly\n";
		}
	}
	else
	{
		std::cout << "ERROR: One or more of the shaders was invalid.\n";
	}

	if (foundProblem)
	{
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(shaderProgram);
}

void ShaderProgram::Use()
{
	glUseProgram(shaderProgram);
}

void ShaderProgram::ReloadShader()
{
	glDeleteProgram(shaderProgram);

	m_vertShader->Reload();
	m_fragShader->Reload();

	shaderProgram = glCreateProgram();

	bool foundProblem = false;

	if (m_vertShader->ShaderLoaded() && m_fragShader->ShaderLoaded())
	{
		glAttachShader(shaderProgram, m_vertShader->GetShaderID());
		glAttachShader(shaderProgram, m_fragShader->GetShaderID());

		glLinkProgram(shaderProgram);

		GLchar errorLog[512];
		GLint success = 0;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cout << "Error linking the shaders.\n";
			glGetProgramInfoLog(shaderProgram, 512, nullptr, errorLog);
			std::cout << errorLog << '\n';
			foundProblem = true;
		}
		else
		{
			std::cout << "Shader Reloaded\n\n";
		}
	}
	else
	{
		std::cout << "ERROR: One or more of the shaders was invalid.\n";
	}

	if (foundProblem)
	{
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}

	ApplyUniforms(); // Reapply uniforms
}


void ShaderProgram::BindUniform(std::string varName, int value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform1i(varLoc, value);
}

void ShaderProgram::BindUniform(std::string varName, float value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform1f(varLoc, value);
}

void ShaderProgram::BindUniform(std::string varName, glm::vec3 value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform3f(varLoc, value.x, value.y, value.z);
}

void ShaderProgram::BindUniform(std::string varName, glm::mat4 value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniformMatrix4fv(
		varLoc,
		1,
		GL_FALSE,		// Don't want to transpose
		&value[0][0]);	// Pointer to first float in mat
}

void ShaderProgram::BindArrayUniform(std::string varName, std::vector<int> value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform1iv(varLoc, value.size(), value.data());
}

void ShaderProgram::BindArrayUniform(std::string varName, std::vector<float> value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform1fv(varLoc, value.size(), value.data());
}

void ShaderProgram::BindArrayUniform(std::string varName, std::vector<glm::vec3> value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform3fv(varLoc, value.size(), (float*)value.data());
}

void ShaderProgram::BindArrayUniform(std::string varName, std::vector<glm::mat4> value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniformMatrix4fv(
		varLoc,
		value.size(),
		GL_FALSE,			// Don't want to transpose
		&value[0][0][0]);	// Pointer to first float in mat
}

void ShaderProgram::BindArrayElementUniform(std::string varName, int index, int value)
{
	std::string indexText = "[" + std::to_string(index) + "]";
	GLint varLoc = glGetUniformLocation(shaderProgram, (varName + indexText).c_str());
	glUniform1iv(varLoc, 1, &value);
}

void ShaderProgram::BindArrayElementUniform(std::string varName, int index, float value)
{
	std::string indexText = "[" + std::to_string(index) + "]";
	GLint varLoc = glGetUniformLocation(shaderProgram, (varName + indexText).c_str());
	glUniform1fv(varLoc, 1, &value);
}

void ShaderProgram::BindArrayElementUniform(std::string varName, int index, glm::vec3 value)
{
	std::string indexText = "[" + std::to_string(index) + "]";
	GLint varLoc = glGetUniformLocation(shaderProgram, (varName + indexText).c_str());
	glUniform3fv(varLoc, 1, (float*)&value);
}

void ShaderProgram::BindArrayElementUniform(std::string varName, int index, glm::mat4 value)
{
	std::string indexText = "[" + std::to_string(index) + "]";
	GLint varLoc = glGetUniformLocation(shaderProgram, (varName + indexText).c_str());
	glUniformMatrix4fv(
		varLoc,
		1,
		GL_FALSE,		// Don't want to transpose
		&value[0][0]);	// Pointer to first float in mat
}


void ShaderProgram::ApplyUniforms()
{
	Use();

	m_uniforms.ApplyAll(*this);
}
