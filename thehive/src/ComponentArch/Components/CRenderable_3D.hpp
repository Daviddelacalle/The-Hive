#ifndef _CRENDERABLE_3D_H
#define _CRENDERABLE_3D_H

#include <ComponentArch/IComponent.hpp>
#include <ComponentArch/Message.hpp>
#include <GameEngine/Model.hpp>
#include <map>
#include <iostream>
#include <Singleton.hpp>


class CTransform;

class CRenderable_3D : public IComponent {
    public:
        CRenderable_3D(const std::string &pathToModel, const Material &material);
        CRenderable_3D(const CRenderable_3D &orig);
        virtual ~CRenderable_3D();

        // Functions of IComponent
        virtual gg::EMessageStatus processMessage(const Message &m);
        virtual void Init();
        virtual void Update();

        //  Handler
        gg::EMessageStatus MHandler_SETPTRS();
        void setVisibility(bool flag);


    private:
        Model _3DModel;

        CTransform* cTransform;
};

#endif
