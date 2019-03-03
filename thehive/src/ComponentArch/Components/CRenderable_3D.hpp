#ifndef _CRENDERABLE_3D_H
#define _CRENDERABLE_3D_H

#include <ComponentArch/IComponent.hpp>
#include <ComponentArch/Message.hpp>
#include <SurrealEngine/ZStaticMesh.hpp>
#include <SurrealEngine/ZMaterial.hpp>
#include <map>
#include <iostream>
#include <Singleton.hpp>
#include <SurrealEngine/TMotorTAG.hpp>


class CTransform;

class CRenderable_3D : public IComponent {
    public:
        CRenderable_3D(const std::string &pathToModel, ZMaterial* material);
        CRenderable_3D(const CRenderable_3D &orig);
        virtual ~CRenderable_3D();

        // Functions of IComponent
        virtual gg::EMessageStatus processMessage(const Message &m);
        virtual void Init();
        virtual void Update();

        //  Handler
        gg::EMessageStatus MHandler_SETPTRS();

    private:
        TNodo* _3DModel;
        TMotorTAG* Surreal;

        CTransform* cTransform;
};

#endif
