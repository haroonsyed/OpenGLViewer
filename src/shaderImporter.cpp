#include "shaderImporter.h"
#include <fstream>

std::string ShaderImporter::readShader(std::string filepath) {
	std::ifstream file(filepath);
	std::string shader;
	
	std::getline(file, shader, '\0');

	return shader;
}