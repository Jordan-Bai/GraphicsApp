#include "Utilities.h"

#include <fstream>

std::string LoadFileAsString(std::string fileName)
{
	std::string output;

	std::ifstream ifs(fileName);

	//std::streambuf* buffer = ifs.rdbuf();
	//char* test = new char[ifs.gcount()];
	//buffer->sgetn(test, ifs.gcount());
	//
	//output = test;

	std::string line;
	
	while (std::getline(ifs, line))
	{
		output += line + '\n';
	}

	return output;
}
