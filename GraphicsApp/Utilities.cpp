#include "Utilities.h"

#include "ShaderProgram.h"
#include <fstream>

std::string LoadFileAsString(std::string fileName)
{
	std::string output;

	std::ifstream ifs(fileName);

	std::string line;
	
	while (std::getline(ifs, line))
	{
		output += line + '\n';
	}

	return output;
}

void UniformStorage::SetUniform(std::string varName, int value)
{
	m_ints[varName] = value;
}

void UniformStorage::SetUniform(std::string varName, float value)
{
	m_floats[varName] = value;
}

void UniformStorage::SetUniform(std::string varName, glm::vec3 value)
{
	m_vec3s[varName] = value;
}

void UniformStorage::SetUniform(std::string varName, glm::mat4 value)
{
	m_mat4s[varName] = value;
}

void UniformStorage::SetArrayUniform(std::string varName, std::vector<int> value)
{
	m_intArrays[varName] = value;
}

void UniformStorage::SetArrayUniform(std::string varName, std::vector<float> value)
{
	m_floatArrays[varName] = value;
}

void UniformStorage::SetArrayUniform(std::string varName, std::vector<glm::vec3> value)
{
	m_vec3Arrays[varName] = value;
}

void UniformStorage::SetArrayUniform(std::string varName, std::vector<glm::mat4> value)
{
	m_mat4Arrays[varName] = value;
}


void UniformStorage::ApplyAll(ShaderProgram& shader)
{
	// Ints
	std::map<std::string, int>::iterator intIt;
	for (intIt = m_ints.begin(); intIt != m_ints.end(); intIt++)
	{
		shader.BindUniform(intIt->first, intIt->second);
	}
	std::map<std::string, std::vector<int>>::iterator intArrayIt;
	for (intArrayIt = m_intArrays.begin(); intArrayIt != m_intArrays.end(); intArrayIt++)
	{
		shader.BindArrayUniform(intArrayIt->first, intArrayIt->second);
	}

	// Floats
	std::map<std::string, float>::iterator floatIt;
	for (floatIt = m_floats.begin(); floatIt != m_floats.end(); floatIt++)
	{
		shader.BindUniform(floatIt->first, floatIt->second);
	}
	std::map<std::string, std::vector<float>>::iterator floatArrayIt;
	for (floatArrayIt = m_floatArrays.begin(); floatArrayIt != m_floatArrays.end(); floatArrayIt++)
	{
		shader.BindArrayUniform(floatArrayIt->first, floatArrayIt->second);
	}

	// Vec3s
	std::map<std::string, glm::vec3>::iterator vec3It;
	for (vec3It = m_vec3s.begin(); vec3It != m_vec3s.end(); vec3It++)
	{
		shader.BindUniform(vec3It->first, vec3It->second);
	}
	std::map<std::string, std::vector<glm::vec3>>::iterator vec3ArrayIt;
	for (vec3ArrayIt = m_vec3Arrays.begin(); vec3ArrayIt != m_vec3Arrays.end(); vec3ArrayIt++)
	{
		shader.BindArrayUniform(vec3ArrayIt->first, vec3ArrayIt->second);
	}

	// Mat4s
	std::map<std::string, glm::mat4>::iterator mat4It;
	for (mat4It = m_mat4s.begin(); mat4It != m_mat4s.end(); mat4It++)
	{
		shader.BindUniform(mat4It->first, mat4It->second);
	}
	std::map<std::string, std::vector<glm::mat4>>::iterator mat4ArrayIt;
	for (mat4ArrayIt = m_mat4Arrays.begin(); mat4ArrayIt != m_mat4Arrays.end(); mat4ArrayIt++)
	{
		shader.BindArrayUniform(mat4ArrayIt->first, mat4ArrayIt->second);
	}
}
