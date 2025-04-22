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

	//===================================
	// HOW DELETE VERT AND FRAG SHADERS?
	//===================================
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



void ShaderProgram::SetUniform(std::string varName, float value)
{
	m_uniformFloats[varName] = value;
}

void ShaderProgram::SetUniform(std::string varName, glm::vec3 value)
{
	m_uniformVec3s[varName] = value;
}

void ShaderProgram::SetUniform(std::string varName, glm::mat4 value)
{
	m_uniformMat4s[varName] = value;
}

void ShaderProgram::SetUniform(std::string varName, int value)
{
	m_uniformInts[varName] = value;
}

void ShaderProgram::SetArrayUniform(std::string varName, int count, std::vector<glm::vec3> valueArray)
{
	m_uniformVec3Arrays[varName] = valueArray;
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

void ShaderProgram::BindUniform(std::string varName, int value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform1i(varLoc, value);
}

void ShaderProgram::BindArrayUniform(std::string varName, int count, glm::vec3* valueArray)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform3fv(varLoc, count, (float*)valueArray);
}

void ShaderProgram::BindArrayUniform(std::string varName, std::vector<glm::vec3> value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform3fv(varLoc, value.size(), (float*)value.data());
}


void ShaderProgram::ApplyUniforms()
{
	Use();

	std::map<std::string, float>::iterator floatIt;
	for (floatIt = m_uniformFloats.begin(); floatIt != m_uniformFloats.end(); floatIt++)
	{
		BindUniform((*floatIt).first, (*floatIt).second);
	}

	std::map<std::string, glm::vec3>::iterator vec3It;
	for (vec3It = m_uniformVec3s.begin(); vec3It != m_uniformVec3s.end(); vec3It++)
	{
		BindUniform((*vec3It).first, (*vec3It).second);
	}

	std::map<std::string, std::vector<glm::vec3>>::iterator vec3ArrayIt;
	for (vec3ArrayIt = m_uniformVec3Arrays.begin(); vec3ArrayIt != m_uniformVec3Arrays.end(); vec3ArrayIt++)
	{
		BindArrayUniform((*vec3ArrayIt).first, (*vec3ArrayIt).second.size(), (*vec3ArrayIt).second.data());
	}

	std::map<std::string, glm::mat4>::iterator mat4It;
	for (mat4It = m_uniformMat4s.begin(); mat4It != m_uniformMat4s.end(); mat4It++)
	{
		BindUniform((*mat4It).first, (*mat4It).second);
	}

	std::map<std::string, int>::iterator intIt;
	for (intIt = m_uniformInts.begin(); intIt != m_uniformInts.end(); intIt++)
	{
		BindUniform((*intIt).first, (*intIt).second);
	}
}
