#pragma once
#include <string>
#include <fstream>

std::string LoadFileAsString(std::string fileName)
{
	std::string output;

	std::ifstream ifs(fileName);
	//ifs.open(fileName, std::ifstream::in);

	std::string line;

	while (std::getline(ifs, line))
	{
		output += line;
	}

	return output;
}