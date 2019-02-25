#ifndef ZMESHDATA_H
#define ZMESHDATA_H

#include <vector>
#include <cstdint>
#include <string>

class ZMeshData{

    public:
        ZMeshData();
        ZMeshData(const ZMeshData &orig);
        ~ZMeshData();

        bool load(const std::string& path);
        void draw();

        unsigned int VAO;
    private:
        unsigned int IndexSize;
        std::vector<unsigned int> VBOs;

        void addVertexBuffer(std::vector<float>& data, unsigned int DataLength);
};

#endif