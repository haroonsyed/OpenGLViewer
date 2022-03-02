#pragma once
#include <string>
#include <vector>

class MeshImporter {
  private:
    std::vector<float> getIndexedPosition(std::vector<float> positions, int index);
    std::vector<std::string> delimit(std::string str, char delimiter); //Probably better in a string class
    void normalizeMesh(std::vector<float>& vertexIndex);
  public:
    std::vector<float> readMesh(std::string filepath);

};
