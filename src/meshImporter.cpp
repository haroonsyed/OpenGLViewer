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

// Used to build the index of vertex data for an attribute. The actual indices themseleves are from buildFaces
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

            // Get each position (list of 3 coordinates)
            attributeIndex.push_back((type)std::stof(delimited[1]));
            attributeIndex.push_back((type)std::stof(delimited[2]));
            attributeIndex.push_back((type)std::stof(delimited[3]));

        }

    }

    return attributeIndex;
}

// Given a string containing data vertex data in format "pos//tex//norm" returns delimited list
std::vector<unsigned int> MeshImporter::buildVertex(std::string vertex) {

    StringUtil sutil;
    std::vector<std::string> delimited = sutil.delimit(vertex, '/');
    std::vector<unsigned int> vertexIndices;

    for (std::string indice : delimited) {
        vertexIndices.push_back(std::stoul(indice) - 1); // -1 to account for obj index start at 1 instead of 0
    }

    return vertexIndices;
}

// Given a line containing an ngon, breaks into tris and adds vertices to list
// Output format is vector of vertices each containing a vector of vertex data
std::vector<std::vector<unsigned int>> MeshImporter::buildFace(std::string line) {

    StringUtil sutil;
    std::vector<std::vector<unsigned int>> face;

    auto delimited = sutil.delimit(line, ' ');

    if (delimited[0] != "f") return face; // Return nothing if the line is not face data

    // Loop breaks n-gons into tris
    for (int i = 0; i < delimited.size() - 3; i++) {

        face.push_back(buildVertex(delimited[1]));
        face.push_back(buildVertex(delimited[i + 2]));
        face.push_back(buildVertex(delimited[i + 3]));

    }

    return face;
}

// [vertex][position texture normal] 
std::vector<std::vector<unsigned int>> MeshImporter::buildMesh(std::string meshFilePath) {

    std::ifstream file(meshFilePath);
    StringUtil sutil;
    std::vector<std::vector<unsigned int>> meshData;
    std::string line;

    while (std::getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        auto face = buildFace(line);
        meshData.insert(meshData.end(), face.begin(), face.end());

    }

    return meshData;
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
    std::vector<float> nIndex;
    std::vector<std::vector<unsigned int>> mesh;
    std::vector<float> vertices;

    std::string line;

    vIndex = getVIndex(meshFilePath);
    nIndex = getNormalIndex(meshFilePath);
    mesh = buildMesh(meshFilePath);
    normalizeMesh(vIndex);

    // Go through the vIndex and fIndex and build a separate tri structure with color data
    // Vertex Format: x,y,z,nx,ny,nz,r,g,b

    // MULTI COLOR TRI
    //auto color1 = { 1.0f,0.0f,0.0f };
    //auto color2 = { 0.0f,1.0f,0.0f };
    //auto color3 = { 0.0f,0.0f,1.0f };

    // WHITE
    //auto color1 = { 1.0f,1.0f,1.0f };
    //auto color2 = { 1.0f,1.0f,1.0f };
    //auto color3 = { 1.0f,1.0f,1.0f };

    // RED
    auto color1 = { 1.0f,0.0f,0.0f };
    auto color2 = { 1.0f,0.0f,0.0f };
    auto color3 = { 1.0f,0.0f,0.0f };

    std::vector<std::initializer_list<float>> triColors = { color1, color2, color3 };

    int vertexNumber = 0;
    for (std::vector<unsigned int> vertex : mesh) {

        int normPos = vertex.size() == 3 ? 2 : 1; //  if in pos/tex/norm versus pos//norm format

        // Get coord data from index
        auto pos = getIndexedPosition(vIndex, vertex[0]);
        auto norm = getIndexedPosition(nIndex, vertex[normPos]); // If tex coo
        auto color = triColors[vertexNumber % 3];

        //Insert into vertices, with color data
        vertices.insert(vertices.end(), pos.begin(), pos.end());
        vertices.insert(vertices.end(), norm.begin(), norm.end());
        vertices.insert(vertices.end(), color);

        vertexNumber++;

    }

    return vertices;
}




