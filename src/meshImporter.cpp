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

template <typename type> 
std::vector<type> MeshImporter::getAttributeIndex(std::string meshFilePath, std::string attribute) {
    std::ifstream file(meshFilePath);
    StringUtil sutil;

    std::vector<type> attributeIndex;
    std::string line;

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        auto delimited = sutil.delimit(line, ' ');

        // Build index
        if (delimited[0] == attribute) {

            // Loop also breaks n-gons into tris (other attributes are just left as 3 data points)
            for (int i = 0; i < delimited.size() - 3; i++) {

                // Get each position (list of 3 coordinates)
                attributeIndex.push_back((type)std::stof(delimited[1]));
                attributeIndex.push_back((type)std::stof(delimited[i + 2]));
                attributeIndex.push_back((type)std::stof(delimited[i + 3]));

            }

        }

    }

    return attributeIndex;
}

// Gets all normals corresponding to the vertices in the vIndex
std::vector<float> MeshImporter::getNormalIndex(std::string meshFilePath) {
    std::vector<float> normalIndex = getAttributeIndex<float>(meshFilePath, "vn");

    // If the normal length is zero then try building normals from other meshData


    return normalIndex;
}

std::vector<float> buildNormals(std::string meshFilePath) {
    return { 0,0,0 };
}

/*********************/
// PUBLIC FUNCTIONS
/*********************/

// Returns an index of all the vertices in a mesh
std::vector<float> MeshImporter::getVIndex(std::string meshFilePath)
{
    std::vector<float> vIndex = getAttributeIndex<float>(meshFilePath, "v");

    return vIndex;
}

// Returns an index of all faces, with 3 uints representing position of each vertex for each face in vIndex
std::vector<unsigned int> MeshImporter::getFaceIndex(std::string meshFilePath)
{
    std::vector<unsigned int> vIndex = getAttributeIndex<unsigned int>(meshFilePath, "f");

    return vIndex;
}

// Uses vIndex and fIndex to build a separate tris structure
std::vector<float> MeshImporter::readSepTriMesh(std::string meshFilePath)
{
    std::ifstream file(meshFilePath);
    StringUtil sutil;

    std::vector<float> vIndex;
    std::vector<unsigned int> fIndex;
    std::vector<float> nIndex;
    std::vector<float> vertices;

    std::string line;

    vIndex = getVIndex(meshFilePath);
    fIndex = getFaceIndex(meshFilePath);
    nIndex = getNormalIndex(meshFilePath);
    normalizeMesh(vIndex);

    // Go through the vIndex and fIndex and build a separate tri structure with color data
    // Vertex Format: x1,y1,z1,x2,y2,z2,x3,y3,z3,r1,g1,b1,r2,g2,b2,r3,g3,b3

    //auto color1 = { 1.0f,0.0f,0.0f };
    //auto color2 = { 0.0f,1.0f,0.0f };
    //auto color3 = { 0.0f,0.0f,1.0f };

    //auto color1 = { 1.0f,1.0f,1.0f };
    //auto color2 = { 1.0f,1.0f,1.0f };
    //auto color3 = { 1.0f,1.0f,1.0f };

    auto color1 = { 1.0f,0.0f,0.0f };
    auto color2 = { 1.0f,0.0f,0.0f };
    auto color3 = { 1.0f,0.0f,0.0f };

    for (int i = 0; i < fIndex.size() / 3; i++) { // Since each face has 3 positions we div by 3

        // Each face is built from 3 vertices. First find position of attribute in index
        // Sub 1 from fIndex to account for obj vIndex starting at 1 instead of 0
        unsigned int v1IndexPos = fIndex[3 * i + 0] - 1;
        unsigned int v2IndexPos = fIndex[3 * i + 1] - 1;
        unsigned int v3IndexPos = fIndex[3 * i + 2] - 1;

        // Get each position (list of 3 coordinates)
        auto v1 = getIndexedPosition(vIndex, v1IndexPos);
        auto v2 = getIndexedPosition(vIndex, v2IndexPos);
        auto v3 = getIndexedPosition(vIndex, v3IndexPos);

        // Get each normal (list of x,y,z component)
        auto n1 = getIndexedPosition(nIndex, v1IndexPos);
        auto n2 = getIndexedPosition(nIndex, v2IndexPos);
        auto n3 = getIndexedPosition(nIndex, v3IndexPos);

        //Insert into vertices, with color data
        vertices.insert(vertices.end(), v1.begin(), v1.end());
        vertices.insert(vertices.end(), n1.begin(), n1.end());
        vertices.insert(vertices.end(), color1);

        vertices.insert(vertices.end(), v2.begin(), v2.end());
        vertices.insert(vertices.end(), n2.begin(), n2.end());
        vertices.insert(vertices.end(), color2);

        vertices.insert(vertices.end(), v3.begin(), v3.end());
        vertices.insert(vertices.end(), n3.begin(), n3.end());
        vertices.insert(vertices.end(), color3);
    }

    return vertices;
}




