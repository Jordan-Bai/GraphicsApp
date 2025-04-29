#pragma once

#include "Shader.h"
#include "Utilities.h"

class ShaderProgram
{
private:
	GLuint m_shaderProgram = 0;

	Shader* m_vertShader = nullptr;
	Shader* m_fragShader = nullptr;

public:
	UniformStorage m_uniforms; // Stores uniforms, so if the shader is ever reloaded, the uniforms can be reapplied

	ShaderProgram(Shader* vertexShader, Shader* fragmentShader);
	~ShaderProgram();

	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;

	void Use();
	void ReloadShader();

	void BindUniform(std::string varName, int value); // Also includes binding samplers
	void BindUniform(std::string varName, float value);
	void BindUniform(std::string varName, glm::vec3 value);
	void BindUniform(std::string varName, glm::mat4 value);
	void BindUniform(std::string varName, std::vector<int> value);
	void BindUniform(std::string varName, std::vector<float> value);
	void BindUniform(std::string varName, std::vector<glm::vec3> value);
	void BindUniform(std::string varName, std::vector<glm::mat4> value);
	// Bind a single element of a uniform array
	void BindUniformElement(std::string varName, int index, int value);
	void BindUniformElement(std::string varName, int index, float value);
	void BindUniformElement(std::string varName, int index, glm::vec3 value);
	void BindUniformElement(std::string varName, int index, glm::mat4 value);

	void ApplyUniforms();
};