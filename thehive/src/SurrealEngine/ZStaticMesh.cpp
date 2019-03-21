#include "ZStaticMesh.hpp"
#include "AssetManager.hpp"

#include <glm/gtx/norm.hpp>
#include <iostream>
#include <sys/stat.h>

#define LOD1 10000
#define KILL 22500

glm::vec3* ZStaticMesh::PlayerPosition;

ZStaticMesh::ZStaticMesh()
:zmat(nullptr)
{

}

bool ZStaticMesh::load(const std::string& Name){
    MeshLODs.reserve(2);
    MeshLODs.push_back(AssetManager::getMeshData(Name));
    return true;
}

void ZStaticMesh::addLOD(std::string Name){
    //Check if file exists
    struct stat buffer;
    if(stat (Name.c_str(), &buffer) == 0) {
        MeshLODs.push_back(AssetManager::getMeshData(Name));
    }
}


void ZStaticMesh::assignMaterial(ZMaterial* material_){
    zmat = material_;
}

void ZStaticMesh::setPlayerPtr(glm::vec3* _PlayerPosition){
    PlayerPosition = _PlayerPosition;
}


void ZStaticMesh::beginDraw(){

    uint8_t LOD = 0;

    glm::vec3 ObjectPos(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
    float distance = glm::length2(ObjectPos-(*PlayerPosition));
    if(distance > KILL) return;
    else if(distance > LOD1 && MeshLODs.size() > 1) LOD = 1;

    Shader* sh = zmat->getShader();

    // MODELO
    GLuint M = sh->getUniformLocation("M");
    glUniformMatrix4fv(M,1,GL_FALSE,&modelMatrix[0][0]);

    // MODELO*VISTA*PERSPECTIVA
    glm::mat4 MVP_L = projMatrix * viewMatrix * modelMatrix;
    GLuint MVP = sh->getUniformLocation("MVP");
    glUniformMatrix4fv(MVP,1,GL_FALSE,&MVP_L[0][0]);

    // LA FINALE
    zmat->Bind();
    MeshLODs[LOD]->draw();
}

void ZStaticMesh::endDraw(){}

ZStaticMesh::~ZStaticMesh(){
    ////std::cout << "Deleting mesh..." << '\n';
}
