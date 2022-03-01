#include "meshImporter.h"

#include <fstream>
#include <iostream>

std::vector<std::string> MeshImporter::delimit(std::string str, char delimiter) {
    std::vector<std::string> delimited;
    std::string curr = "";
    for (char c : str) {

        if (c == delimiter) {
            delimited.push_back(curr);
            curr = "";
        }
        else {
            curr = curr + c;
        }

    }

    if (curr.size() > 0) {
        //Add the last element
        delimited.push_back(curr);
    }

    return delimited;
}

// CURRENTLY EXPECTS TRIS ONLY
std::vector<float> MeshImporter::readMesh(std::string filepath)
{
    std::ifstream file(filepath);
    std::vector<float> vIndex;
    std::vector<float> vertices;

    std::string line;
    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }


        auto delimited = delimit(line, ' ');

        //DEBUG
        //std::cout << line << std::endl;
        /*for(std::string s : delimited) {
          std::cout << s << std::endl;
        }*/

        // Build index
        if(delimited[0] == "v") {
          vIndex.push_back(std::stof(delimited[1]));
          vIndex.push_back(std::stof(delimited[2]));
          vIndex.push_back(std::stof(delimited[3]));
        }

        // Build tris from faces, interpolated with color data
        else if(delimited[0] == "f") {
          vertices.push_back(vIndex.at( delimited[1][0]-'0' ));
          vertices.insert(vertices.end(), {1.0f,0.0f,0.0f});
          vertices.push_back(vIndex.at( delimited[2][0]-'0' ));
          vertices.insert(vertices.end(), {0.0f,1.0f,0.0f});
          vertices.push_back(vIndex.at( delimited[3][0]-'0' ));
          vertices.insert(vertices.end(), {0.0f,0.0f,1.0f});
        }

    }

    return vertices;
}
