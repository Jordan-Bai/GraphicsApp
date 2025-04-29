#pragma once
#include <string>
#include <map>
#include <vector>
#include "glm.hpp"

std::string LoadFileAsString(std::string fileName);

struct Vertex
{
	Vertex()
		: pos(), normal(), tangent(), UVcoord()
	{
	}
	Vertex(glm::vec3 _pos)
		: pos(_pos), normal(), tangent(), UVcoord()
	{
	}
	Vertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec3 _tangent)
		: pos(_pos), normal(_normal), tangent(_tangent), UVcoord()
	{
	}
	Vertex(glm::vec3 _pos, glm::vec3 _normal, glm::vec3 _tangent, glm::vec2 _uvCoord)
		: pos(_pos), normal(_normal), tangent(_tangent), UVcoord(_uvCoord)
	{
	}

	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 UVcoord;
};

class ShaderProgram;

struct UniformStorage
{
	std::map<std::string, int> m_ints;
	std::map<std::string, float> m_floats;
	std::map<std::string, glm::vec3> m_vec3s;
	std::map<std::string, glm::mat4> m_mat4s;
	std::map<std::string, std::vector<int>> m_intArrays;
	std::map<std::string, std::vector<float>> m_floatArrays;
	std::map<std::string, std::vector<glm::vec3>> m_vec3Arrays;
	std::map<std::string, std::vector<glm::mat4>> m_mat4Arrays;

	// Makes things a bit easier since you don't need to type out the correct variable name
	void SetUniform(std::string varName, int value); // Also includes setting samplers
	void SetUniform(std::string varName, float value);
	void SetUniform(std::string varName, glm::vec3 value);
	void SetUniform(std::string varName, glm::mat4 value);
	void SetUniform(std::string varName, std::vector<int> value);
	void SetUniform(std::string varName, std::vector<float> value);
	void SetUniform(std::string varName, std::vector<glm::vec3> value);
	void SetUniform(std::string varName, std::vector<glm::mat4> value);
	// Set a single element of a uniform array
	void SetUniformElement(std::string varName, int index, int value);
	void SetUniformElement(std::string varName, int index, float value);
	void SetUniformElement(std::string varName, int index, glm::vec3 value);
	void SetUniformElement(std::string varName, int index, glm::mat4 value);

	void ApplyAll(ShaderProgram& shader);
};