#pragma once

#include "Shader.h"
#include "Utilities.h"
//#include "glm.hpp"
//#include "Texture.h"
//#include <string>
//#include <map>
//#include <vector>

class ShaderProgram
{
private:
	GLuint shaderProgram = 0;

	//GLuint vertexShader = 0;
	//GLuint fragmentShader = 0;

	Shader* m_vertShader = nullptr;
	Shader* m_fragShader = nullptr;

	std::string m_vertexFileName;
	std::string m_fragmentFileName;

public:
	//std::map<std::string, float> m_uniformFloats;
	//std::map<std::string, glm::vec3> m_uniformVec3s;
	//std::map<std::string, glm::mat4> m_uniformMat4s;
	//std::map<std::string, int> m_uniformInts;
	//std::map<std::string, std::vector<glm::vec3>> m_uniformVec3Arrays;
	UniformStorage m_uniforms;

	//ShaderProgram() {}
	ShaderProgram(Shader* vertexShader, Shader* fragmentShader);
	ShaderProgram(std::string vertexFilename, std::string fragmentFilename);

	~ShaderProgram();

	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;

	void Use();
	void ReloadShader();

	//void SetUniform(std::string varName, int value); // Also includes setting samplers
	//void SetUniform(std::string varName, float value);
	//void SetUniform(std::string varName, glm::vec3 value);
	//void SetUniform(std::string varName, glm::mat4 value);
	//void SetArrayUniform(std::string varName, std::vector<int> value);
	//void SetArrayUniform(std::string varName, std::vector<float> value);
	//void SetArrayUniform(std::string varName, std::vector<glm::vec3> value);
	//void SetArrayUniform(std::string varName, std::vector<glm::mat4> value);

	void BindUniform(std::string varName, int value); // Also includes binding samplers
	void BindUniform(std::string varName, float value);
	void BindUniform(std::string varName, glm::vec3 value);
	void BindUniform(std::string varName, glm::mat4 value);
	void BindArrayUniform(std::string varName, std::vector<int> value);
	void BindArrayUniform(std::string varName, std::vector<float> value);
	void BindArrayUniform(std::string varName, int count, glm::vec3* valueArray);
	void BindArrayUniform(std::string varName, std::vector<glm::vec3> value);
	void BindArrayUniform(std::string varName, std::vector<glm::mat4> value);

	void ApplyUniforms();
};