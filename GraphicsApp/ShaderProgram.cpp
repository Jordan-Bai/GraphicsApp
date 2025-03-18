#include "ShaderProgram.h"
#include "Utilities.h"
#include <iostream>

ShaderProgram::ShaderProgram(std::string vertexFilename, std::string fragmentFilename)
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();

	bool foundProblem = false;

	std::string vertexSource = LoadFileAsString(vertexFilename);
	std::string fragmentSource = LoadFileAsString(fragmentFilename);
	if (vertexSource == "" || fragmentSource == "")
	{
		std::cout << "Failed to open one or more shader source files.\n";
		std::cout << "Is your working directory set up correctly?\n";
		foundProblem = true;
	}
	else
	{
		const char* vertexSourceC = vertexSource.c_str();

		glShaderSource(vertexShader, 1, &vertexSourceC, nullptr);
		glCompileShader(vertexShader);

		GLchar errorLog[512];
		GLint success = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			std::cout << "Something went wrong with the vertex shader!\n";
			glGetShaderInfoLog(vertexShader, 512, nullptr, errorLog);
			std::cout << errorLog << '\n';
			foundProblem = true;
		}
		else
		{
			std::cout << "Vertex shader \"" << vertexFilename << "\" loaded successfuly!\n";
		}
		const char* fragmentSourceC = fragmentSource.c_str();

		glShaderSource(fragmentShader, 1, &fragmentSourceC, nullptr);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			//Something failed with the fragment shader compilation
			std::cout << "Fragment shader " << fragmentFilename << " failed with error:\n";
			glGetShaderInfoLog(fragmentShader, 512, nullptr, errorLog);
			std::cout << errorLog << '\n';
			foundProblem = true;
		}
		else
		{
			std::cout << "\"" << fragmentFilename << "\" compiled successfully.\n";
		}

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);
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

	if (foundProblem)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);
		vertexShader = 0;
		fragmentShader = 0;
		shaderProgram = 0;
	}
}

ShaderProgram::~ShaderProgram()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
}

void ShaderProgram::Use()
{
	glUseProgram(shaderProgram);
}

void ShaderProgram::SetFloatUniform(std::string varName, float value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform1f(varLoc, value);
}

void ShaderProgram::SetVector3Uniform(std::string varName, glm::vec3 value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform3f(varLoc, value.x, value.y, value.z);
}

void ShaderProgram::SetMatrix4Uniform(std::string varName, glm::mat4 value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniformMatrix4fv(
		varLoc,
		1,
		GL_FALSE,		// Don't want to transpose
		&value[0][0]);	// Pointer to first float in mat
}

void ShaderProgram::SetIntUniform(std::string varName, GLuint value)
{
	GLint varLoc = glGetUniformLocation(shaderProgram, varName.c_str());
	glUniform1i(varLoc, value);
}
