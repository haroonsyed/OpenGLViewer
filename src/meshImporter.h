#pragma once
#include <string>
#include <vector>

class MeshImporter {
  private:
    std::vector<float> getIndexedPosition(std::vector<float> positions, int index);
    void normalizeMesh(std::vector<float>& vertexIndex);
    std::vector<float> getNormalIndex(std::string meshFilePath);
    template <typename type> std::vector<type> getAttributeIndex(std::string meshFilePath, std::string attribute);
    std::vector<std::vector<unsigned int>> MeshImporter::buildMesh(std::string meshFilePath);
  public:
    std::vector<float> readSepTriMesh(std::string meshFilePath);
    std::vector<float> getVIndex(std::string meshFilePath);
    std::vector<unsigned int> getFaceIndex(std::string meshFilePath);
};
