#include "meshImporter.h"

#include <fstream>
#include <iostream>
#include "util.h"

/*********************/
// PRIVATE FUNCTIONS
/*********************/

// Returns the x,y,z coordinates of vertex n from vIndex
std::vector<float> MeshImporter::getIndexedPosition(std::vector<float> vIndex, int index)
{
    std::vector<float> vertex;
    // Get three vertices and return them 
    vertex.push_back(vIndex[index * 3 + 0]);
    vertex.push_back(vIndex[index * 3 + 1]);
    vertex.push_back(vIndex[index * 3 + 2]);

    return vertex;
}

// Divides vIndex by maximum coordinate within it
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

/*********************/
// PUBLIC FUNCTIONS
/*********************/

// Returns an index of all the vertices in a mesh
std::vector<float> MeshImporter::getVIndex(std::string meshFilePath)
{
    std::ifstream file(meshFilePath);
    StringUtil sutil;

    std::vector<float> vIndex;
    std::string line;

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }


        auto delimited = sutil.delimit(line, ' ');

        // Build index
        if (delimited[0] == "v") {
            vIndex.push_back(std::stof(delimited[1]));
            vIndex.push_back(std::stof(delimited[2]));
            vIndex.push_back(std::stof(delimited[3]));
        }

    }

    return vIndex;
}


// Builds an index of faces with vertex positions corresponding to the vIndex
// Also breaks down ngons into tris
std::vector<unsigned int> MeshImporter::getFaceIndex(std::string meshFilePath)
{
    std::ifstream file(meshFilePath);
    StringUtil sutil;

    std::vector<unsigned int> faces;
    std::string line;

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        auto delimited = sutil.delimit(line, ' ');

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

// Uses vIndex and fIndex to build a separate tris structure
std::vector<float> MeshImporter::readSepTriMesh(std::string meshFilePath)
{
    std::ifstream file(meshFilePath);
    StringUtil sutil;

    std::vector<float> vIndex;
    std::vector<unsigned int> fIndex;
    std::vector<float> vertices;

    std::string line;

    vIndex = getVIndex(meshFilePath);
    fIndex = getFaceIndex(meshFilePath);
    normalizeMesh(vIndex);

    // Go through the vIndex and fIndex and build a separate tri structure with color data
    // Vertex Format: x1,y1,z1,x2,y2,z2,x3,y3,z3,r1,g1,b1,r2,g2,b2,r3,g3,b3

    auto color1 = { 1.0f,0.0f,0.0f };
    auto color2 = { 0.0f,1.0f,0.0f };
    auto color3 = { 0.0f,0.0f,1.0f };

    for (int i = 0; i < fIndex.size() / 3; i++) { // Since each face has 3 positions we div by 3

        // Get each position (list of 3 coordinates)
        auto v1 = getIndexedPosition(vIndex, fIndex[3 * i + 0]);
        auto v2 = getIndexedPosition(vIndex, fIndex[3 * i + 1]);
        auto v3 = getIndexedPosition(vIndex, fIndex[3 * i + 2]);

        //Insert into vertices, with color data
        vertices.insert(vertices.end(), v1.begin(), v1.end());
        vertices.insert(vertices.end(), color1);

        vertices.insert(vertices.end(), v2.begin(), v2.end());
        vertices.insert(vertices.end(), color2);

        vertices.insert(vertices.end(), v3.begin(), v3.end());
        vertices.insert(vertices.end(), color3);
    }

    return vertices;
}




