#ifndef BOTON2D_H
#define BOTON2D_H

#include <vector>
#include <cstdint>
#include "ZMaterial.hpp"
#include "TNodo.hpp"
#include "Omicron/Imagen2D.hpp"
#include "Omicron/Texto2D.hpp"
#include <GameEngine/EnumButtonType.hpp>



class Boton2D {
    public:
        //Boton2D();

        Boton2D(float x,float y,float w,float h,EnumButtonType _tipo,const std::string &imgP,const std::string &imgS,const std::string &texto,bool focus=false,glm::vec4 color=glm::vec4(1,0,0,1),float tam=30);
        void setSesgo(float res);
        Boton2D(const Boton2D &orig) = delete;
        ~Boton2D();

        void Draw();
        //void init();
        void setPos(float x,float y,float w,float h);
        void setImage(const std::string &Name);

        void setColor(glm::vec4 _color);
        bool checkOn(float x,float y);
        void hover(bool dato);
        int getType();
        bool getHov();



    private:
            bool hasText;
            Texto2D text;
            Imagen2D Imagen;
            EnumButtonType tipo;
            //std::string imagenes[2];
            bool hov;
            std::string imgPrincipal;
            std::string imgHover_Selected;
            bool selected;
            float X,Y,W,H;
};

#endif