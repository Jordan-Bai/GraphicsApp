#include "ShaderProgram.h"
#include "Utilities.h"
#include <iostream>

ShaderProgram::ShaderProgram(Shader* vertexShader, Shader* fragmentShader)
{
	if (vertexShader == nullptr || fragmentShader == nullptr)
	{
		std::cout << "ERROR(ShaderProgram::ShaderProgram): One or more of the shaders provided was null.\n";
	}
	m_vertShader = vertexShader;
	m_fragShader = fragmentShader;

	m_shaderProgram = glCreateProgram();

	bool foundProblem = false;

	if (m_vertShader->ShaderLoaded() && m_fragShader->ShaderLoaded())
	{
		glAttachShader(m_shaderProgram, m_vertShader->GetShaderID());
		glAttachShader(m_shaderProgram, m_fragShader->GetShaderID());

		glLinkProgram(m_shaderProgram);

		GLchar errorLog[512];
		GLint success = 0;
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cout << "Error linking the shaders.\n";
			glGetProgramInfoLog(m_shaderProgram, 512, nullptr, errorLog);
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
		std::cout << "ERROR(ShaderProgram::ShaderProgram): One or more of the shaders was invalid.\n";
	}

	if (foundProblem)
	{
		glDeleteProgram(m_shaderProgram);
		m_shaderProgram = 0;
	}
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_shaderProgram);
}


void ShaderProgram::Use()
{
	glUseProgram(m_shaderProgram);
}

void ShaderProgram::ReloadShader()
{
	glDeleteProgram(m_shaderProgram);

	m_vertShader->Reload();
	m_fragShader->Reload();

	m_shaderProgram = glCreateProgram();

	bool foundProblem = false;

	if (m_vertShader->ShaderLoaded() && m_fragShader->ShaderLoaded())
	{
		glAttachShader(m_shaderProgram, m_vertShader->GetShaderID());
		glAttachShader(m_shaderProgram, m_fragShader->GetShaderID());

		glLinkProgram(m_shaderProgram);

		GLchar errorLog[512];
		GLint success = 0;
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			std::cout << "Error linking the shaders.\n";
			glGetProgramInfoLog(m_shaderProgram, 512, nullptr, errorLog);
			std::cout << errorLog << '\n';
			foundProblem = true;
		}
		else
		{
			std::cout << "Shader Reloaded\n";
		}
	}
	else
	{
		std::cout << "ERROR(ShaderProgram::ShaderProgram): One or more of the shaders was invalid.\n";
	}

	if (foundProblem)
	{
		glDeleteProgram(m_shaderProgram);
		m_shaderProgram = 0;
	}

	ApplyUniforms(); // Reapply uniforms
}


void ShaderProgram::BindUniform(std::string varName, int value)
{
	GLint varLoc = glGetUniformLocation(m_shaderProgram, varName.c_str());
	glUniform1i(varLoc, value);
}

void ShaderProgram::BindUniform(std::string varName, float value)
{
	GLint varLoc = glGetUniformLocation(m_shaderProgram, varName.c_str());
	glUniform1f(varLoc, value);
}

void ShaderProgram::BindUniform(std::string varName, glm::vec3 value)
{
	GLint varLoc = glGetUniformLocation(m_shaderProgram, varName.c_str());
	glUniform3f(varLoc, value.x, value.y, value.z);
}

void ShaderProgram::BindUniform(std::string varName, glm::mat4 value)
{
	GLint varLoc = glGetUniformLocation(m_shaderProgram, varName.c_str());
	glUniformMatrix4fv(
		varLoc,
		1,
		GL_FALSE,		// Don't want to transpose
		&value[0][0]);	// Pointer to first float in mat
}

void ShaderProgram::BindUniform(std::string varName, std::vector<int> value)
{
	GLint varLoc = glGetUniformLocation(m_shaderProgram, varName.c_str());
	glUniform1iv(varLoc, (int)value.size(), value.data());
}

void ShaderProgram::BindUniform(std::string varName, std::vector<float> value)
{
	GLint varLoc = glGetUniformLocation(m_shaderProgram, varName.c_str());
	glUniform1fv(varLoc, (int)value.size(), value.data());
}

void ShaderProgram::BindUniform(std::string varName, std::vector<glm::vec3> value)
{
	GLint varLoc = glGetUniformLocation(m_shaderProgram, varName.c_str());
	glUniform3fv(varLoc, (int)value.size(), (float*)value.data());
}

void ShaderProgram::BindUniform(std::string varName, std::vector<glm::mat4> value)
{
	GLint varLoc = glGetUniformLocation(m_shaderProgram, varName.c_str());
	glUniformMatrix4fv(
		varLoc,
		(int)value.size(),
		GL_FALSE,			// Don't want to transpose
		&value[0][0][0]);	// Pointer to first float in mat
}

void ShaderProgram::BindUniformElement(std::string varName, int index, int value)
{
	std::string indexText = "[" + std::to_string(index) + "]";
	GLint varLoc = glGetUniformLocation(m_shaderProgram, (varName + indexText).c_str());
	glUniform1iv(varLoc, 1, &value);
}

void ShaderProgram::BindUniformElement(std::string varName, int index, float value)
{
	std::string indexText = "[" + std::to_string(index) + "]";
	GLint varLoc = glGetUniformLocation(m_shaderProgram, (varName + indexText).c_str());
	glUniform1fv(varLoc, 1, &value);
}

void ShaderProgram::BindUniformElement(std::string varName, int index, glm::vec3 value)
{
	std::string indexText = "[" + std::to_string(index) + "]";
	GLint varLoc = glGetUniformLocation(m_shaderProgram, (varName + indexText).c_str());
	glUniform3fv(varLoc, 1, (float*)&value);
}

void ShaderProgram::BindUniformElement(std::string varName, int index, glm::mat4 value)
{
	std::string indexText = "[" + std::to_string(index) + "]";
	GLint varLoc = glGetUniformLocation(m_shaderProgram, (varName + indexText).c_str());
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
