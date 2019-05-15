#ifndef CUADRADO2D_H
#define CUADRADO2D_H

#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

class Shader;

class Cuadrado2D {
    public:
        Cuadrado2D();
        Cuadrado2D(float x,float y,float w,float h);
        Cuadrado2D(const Cuadrado2D &orig);
        ~Cuadrado2D();

        Cuadrado2D& operator=(Cuadrado2D other);

        void Draw();
        void setZindex(float res);

        void setPos(float x,float y,float w,float h);

        void setColor(glm::vec4 _color);


    private:
        unsigned long Zindex;
        unsigned long inputColour;
        Shader* inicio;
        Shader* fin;
        float index;
        glm::vec4 color;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
};

#endif
