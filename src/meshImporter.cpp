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

void MeshImporter::normalizeMesh(std::vector<float>& vIndex)
{
    // Normalize vertices
    float max = 0;
    for (float v : vIndex) {
        max = std::abs(v) > max ? v : max;
    }
    for (int i = 0; i < vIndex.size(); i++) {
        vIndex[i] /= max * 1.1; // 1.1 to keep it better in window instead of exact
    }
}

// CURRENTLY EXPECTS TRIS ONLY
std::vector<float> MeshImporter::readSepTriMesh(std::string meshFilePath)
{
    std::ifstream file(meshFilePath);
    std::vector<float> vIndex;
    std::vector<float> vertices;

    std::string line;

    vIndex = getVIndex(meshFilePath);
    normalizeMesh(vIndex);

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

        // Build tris from faces, interpolated with color data
        if(delimited[0] == "f") {

          auto color1 = { 1.0f,0.0f,0.0f };
          auto color2 = { 0.0f,1.0f,0.0f };
          auto color3 = { 0.0f,0.0f,1.0f };

          // Loop breaks n-gons into tris
          for (int i = 0; i < delimited.size() - 3; i++) {

              // Get each position (list of 3 coordinates)
              auto v1 = getIndexedPosition(vIndex, std::stoi(delimited[1]) - 1);
              auto v2 = getIndexedPosition(vIndex, std::stoi(delimited[i+2]) - 1);
              auto v3 = getIndexedPosition(vIndex, std::stoi(delimited[i+3]) - 1);

              //Insert into vertices, interpolated with color data
              vertices.insert(vertices.end(), v1.begin(), v1.end());
              vertices.insert(vertices.end(), color1);

              vertices.insert(vertices.end(), v2.begin(), v2.end());
              vertices.insert(vertices.end(), color2);

              vertices.insert(vertices.end(), v3.begin(), v3.end());
              vertices.insert(vertices.end(), color3);
          }

        }

    }

    return vertices;
}

// Returns a normalized index of all the vertices
std::vector<float> MeshImporter::getVIndex(std::string meshFilePath)
{
    std::ifstream file(meshFilePath);
    std::vector<float> vIndex;
    std::string line;

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }


        auto delimited = delimit(line, ' ');

        // Build index
        if (delimited[0] == "v") {
            vIndex.push_back(std::stof(delimited[1]));
            vIndex.push_back(std::stof(delimited[2]));
            vIndex.push_back(std::stof(delimited[3]));
        }

    }

    return vIndex;
}

std::vector<unsigned int> MeshImporter::getFaceIndices(std::string, std::string meshFilePath)
{
    std::ifstream file(meshFilePath);
    std::vector<unsigned int> faces;
    std::string line;

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        auto delimited = delimit(line, ' ');

        if (delimited[0] == "f") {

            // Loop breaks n-gons into tris
            for (int i = 0; i < delimited.size() - 3; i++) {

                // Get each position (list of 3 coordinates)
                faces.push_back(std::stoul(delimited[1]) - 1);
                faces.push_back(std::stoul(delimited[i + 2]) - 1);
                faces.push_back(std::stoul(delimited[i + 3]) - 1);

            }

        }

    }

    return faces;
}
