#pragma once

#include "GraphicsHeaders.h"
#include "glm.hpp"
#include "Texture.h"
#include <string>
#include <map>
#include <vector>

class ShaderProgram
{
private:
	GLuint shaderProgram = 0;

	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;

	std::string m_vertexFileName;
	std::string m_fragmentFileName;

public:
	std::map<std::string, float> m_uniformFloats;
	std::map<std::string, glm::vec3> m_uniformVec3s;
	std::map<std::string, std::vector<glm::vec3>> m_uniformVec3Arrays;
	std::map<std::string, glm::mat4> m_uniformMat4s;
	std::map<std::string, int> m_uniformInts;

	ShaderProgram() {}
	ShaderProgram(std::string vertexFilename, std::string fragmentFilename);

	~ShaderProgram();

	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;

	void Use();
	void ReloadShader();

	void SetFloatUniform(std::string varName, float value);
	void SetVector3Uniform(std::string varName, glm::vec3 value);
	void SetVector3ArrayUniform(std::string varName, int count, glm::vec3* valueArray);
	void SetMatrix4Uniform(std::string varName, glm::mat4 value);
	void SetIntUniform(std::string varName, GLuint value); // Also includes setting samplers

	void ApplyUniforms();
};