#pragma once
#include <string>
#include <fstream>

std::string LoadFileAsString(std::string fileName)
{
	std::string output;

	std::ifstream ifs(fileName);

	std::string line;

	while (std::getline(ifs, line))
	{
		output += line;
	}

	return output;
}