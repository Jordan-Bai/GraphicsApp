#pragma once

#include "Shader.h"
#include "Utilities.h"

class ShaderProgram
{
private:
	GLuint shaderProgram = 0;

	Shader* m_vertShader = nullptr;
	Shader* m_fragShader = nullptr;

	std::string m_vertexFileName;
	std::string m_fragmentFileName;

public:
	UniformStorage m_uniforms;

	//ShaderProgram() {}
	ShaderProgram(Shader* vertexShader, Shader* fragmentShader);
	ShaderProgram(std::string vertexFilename, std::string fragmentFilename);

	~ShaderProgram();

	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;

	void Use();
	void ReloadShader();

	void BindUniform(std::string varName, int value); // Also includes binding samplers
	void BindUniform(std::string varName, float value);
	void BindUniform(std::string varName, glm::vec3 value);
	void BindUniform(std::string varName, glm::mat4 value);
	void BindArrayUniform(std::string varName, std::vector<int> value);
	void BindArrayUniform(std::string varName, std::vector<float> value);
	void BindArrayUniform(std::string varName, std::vector<glm::vec3> value);
	void BindArrayUniform(std::string varName, std::vector<glm::mat4> value);

	void ApplyUniforms();
};