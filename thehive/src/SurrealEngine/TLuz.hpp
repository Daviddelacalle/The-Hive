#ifndef TLUZ_H
#define TLUZ_H

#include <Util.hpp>
#include "TEntidad.hpp"
#include "Shader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class TLuz : public TEntidad {
    public:
        TLuz();
        TLuz(gg::Color&);
        TLuz(Shader*);
        TLuz(gg::Color&,Shader*);
        virtual ~TLuz ();

        virtual void setIntensidad(gg::Color&);
        virtual gg::Color getIntensidad();

        virtual void beginDraw(const uint8_t&);
        virtual void endDraw(const uint8_t&);

    private:
        gg::Color intensidad;
        Shader* light_shader;

};

#endif
