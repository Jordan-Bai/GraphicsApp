#pragma once

#include "GraphicsHeaders.h"
#include <string>
#include "glm.hpp"
#include "Texture.h"

class ShaderProgram
{
private:
	GLuint shaderProgram = 0;

	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;

	std::string m_vertexFileName;
	std::string m_fragmentFileName;

public:
	ShaderProgram() {}
	ShaderProgram(std::string vertexFilename, std::string fragmentFilename);

	~ShaderProgram();

	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;

	void Use();
	void ReloadShader();

	void SetFloatUniform(std::string varName, float value);
	void SetVector3Uniform(std::string varName, glm::vec3 value);
	void SetMatrix4Uniform(std::string varName, glm::mat4 value);
	void SetIntUniform(std::string varName, GLuint value); // Also includes setting samplers
};