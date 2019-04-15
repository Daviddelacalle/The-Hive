#ifndef BILLBOARDBUENO_H
#define BILLBOARDBUENO_H

#include <vector>
#include <cstdint>
#include <Omicron/ZMaterial.hpp>
#include <Omicron/CORE/TNodo.hpp>
class Omicron;

class BillboardBueno {
    public:
        //BillboardBueno();
        BillboardBueno(float x,float y,float z,const std::string &Name);

        BillboardBueno(const BillboardBueno &orig) = delete;
        ~BillboardBueno();

        void Draw();
        //void init();
        void setPos(float x,float y,float z);
        void setImage(const std::string &Name);

        void setColor(glm::vec4 _color);
        float getX();
        float getW();
        float getY();
        float getH();

    private:
            Shader* inicio;
            Shader* fin;
            unsigned long MVP                      ;
            unsigned long particleCenter_wordspace ;
            unsigned long CameraRight_worldspace   ;
            unsigned long CameraUp_worldspace      ;
            unsigned long inputColour              ;
            unsigned long loc              ;
            Omicron* engine;
            glm::vec3 centro;
            float index;
            glm::vec4 color;
            unsigned int VAO;
            unsigned int VBO;
            unsigned int EBO;
            unsigned int textureID;
            float X,Y,W,H;

};

#endif