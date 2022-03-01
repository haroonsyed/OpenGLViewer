#include "meshImporter.h"

#include <fstream>
#include <iostream>

std::vector<float> MeshImporter::getIndexedPosition(std::vector<float> positions, int index)
{   
    std::vector<float> vertex;
    // Get three vertices and return them 
    vertex.push_back(positions[index * 3 + 0]);
    vertex.push_back(positions[index * 3 + 1]);
    vertex.push_back(positions[index * 3 + 2]);

    return vertex;
}

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
          // Get each position (list of 3 coordinates)
          auto v1 = getIndexedPosition(vIndex, std::stoi(delimited[1]) - 1);
          auto v2 = getIndexedPosition(vIndex, std::stoi(delimited[2]) - 1);
          auto v3 = getIndexedPosition(vIndex, std::stoi(delimited[3]) - 1);
          
          //Insert into vertices, interpolated with color data
          vertices.insert(vertices.end(), v1.begin(), v1.end());
          vertices.insert(vertices.end(), { 1.0f,0.0f,0.0f });
          
          vertices.insert(vertices.end(), v2.begin(), v2.end());
          vertices.insert(vertices.end(), { 0.0f,1.0f,0.0f });
          
          vertices.insert(vertices.end(), v3.begin(), v3.end());
          vertices.insert(vertices.end(), { 0.0f,0.0f,1.0f });

        }

    }

    return vertices;
}
