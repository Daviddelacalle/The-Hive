#include "MeshImporter.hpp"
#include <iostream>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <Util.hpp>
#include <cmath>

MeshImporter::MeshImporter(){

}


bool MeshImporter::importNavmesh(
    const std::string& pFile,
    std::vector<Node> &GRAPH,
    std::vector<std::vector<Connection>> &Connections
){

    std::vector<gg::Vector3f> vertex;
    std::vector<uint16_t> index;
    std::vector<Edge> Edges;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile( pFile,
    aiProcess_JoinIdenticalVertices  |
    aiProcess_SortByPType);


    if( !scene)
        return false;

    std::cout << "Loading model '" << pFile << "'" << '\n';

    aiMesh **meshes = scene->mMeshes;
    aiVector3D* vertices;
    aiFace* faces;

    vertices   =   meshes[0]->mVertices;
       faces   =   meshes[0]->mFaces;


    std::vector<Edge> v;

    for(uint16_t j = 0; j < meshes[0]->mNumVertices; ++j){
        vertex.emplace_back(vertices[j].x, vertices[j].y, vertices[j].z);
    }

    std::cout << "mNumFaces " << meshes[0]->mNumFaces << '\n';

    Connections.resize(meshes[0]->mNumFaces);

    for(uint16_t j = 0; j < meshes[0]->mNumFaces; ++j) {

        const aiFace& Face = faces[j];

        float X = 0;
        float Y = 0;
        float Z = 0;

        for(uint8_t i = 0; i < Face.mNumIndices; ++i){
            X += vertex[Face.mIndices[i]].X;
            Y += vertex[Face.mIndices[i]].Y;
            Z += vertex[Face.mIndices[i]].Z;
            index.push_back(Face.mIndices[i]);

            Edge NewEdge(Face.mIndices[i], Face.mIndices[(i+1)%Face.mNumIndices], j);

            uint16_t it = 0;
            bool found = false;
            while(it < Edges.size()) {

                if(NewEdge == Edges[it]) {
                    Connections[j].emplace_back(j, Edges[it].face, 0, "[" + std::to_string(j) + "-" + std::to_string(Edges[it].face) + "]");
                    Connections[Edges[it].face].emplace_back(Edges[it].face, j, 0, "[" + std::to_string(Edges[it].face) + "-" + std::to_string(j) + "]");
                    found = true;
                    break;
                }
                ++it;

            }

            if (!found)
                Edges.push_back(NewEdge);
        }

        GRAPH.emplace_back(j, -X/Face.mNumIndices, Y/Face.mNumIndices, Z/Face.mNumIndices);
    }

    if(false) {
        std::cout << '\n' << "VERTEX: " << '\n';
        for(int i = 0; i < vertex.size(); ++i)
            std::cout << " " << i << " -> ("<< vertex[i].X << ", " << vertex[i].Y << ", " << vertex[i].Z  <<")" << '\n';

        std::cout << '\n';

        std::cout << "INDEXES: " << '\n';
        for(int i = 0; i < index.size(); ++i)
            std::cout << " " << index[i];

        std::cout << '\n' << '\n';

        std::cout << "EDGES: " << '\n';
        for(int i = 0; i < Edges.size(); ++i)
            std::cout << " " << Edges[i] << '\n';

        std::cout << '\n' << '\n';

        std::cout << "   |-- VERTEX : " << meshes[0]->mNumVertices << '\n';
        std::cout << "   |--  INDEX : " << index.size() << '\n' << '\n';
    }

    return true;
}


Edge::Edge()
:e1(0), e2(0), face(0)
{}

Edge::Edge(const Edge &orig){
    e1 = orig.e1;
    e2 = orig.e2;
    face = orig.face;
}

Edge::Edge(uint16_t _e1, uint16_t _e2, uint16_t _face)
:e1(_e1), e2(_e2), face(_face)
{}

bool Edge::operator==(const Edge &Edge2){
    return ((e1 == Edge2.e1 && e2 == Edge2.e2) || (e1 == Edge2.e2 && e2 == Edge2.e1));
}

std::ostream& operator<<(std::ostream& os, const Edge &E){
    os << "[" << E.e1 << "," << E.e2 << "]";
    return os;
}
