#pragma once
#include <string>
#include <vector>

class MeshImporter {
  private:
    std::vector<float> getIndexedPosition(std::vector<float> positions, int index);
    void normalizeMesh(std::vector<float>& vertexIndex);
  public:
    std::vector<float> readSepTriMesh(std::string meshFilePath);
    std::vector<float> getVIndex(std::string meshFilePath);
    std::vector<unsigned int> getFaceIndex(std::string meshFilePath);
};
