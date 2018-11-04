#ifndef _CRENDERABLE_3D_H
#define _CRENDERABLE_3D_H

#include "IComponent.hpp"
#include "Model.hpp"

class CRenderable_3D : public IComponent {
    public:
        CRenderable_3D();
        CRenderable_3D(const CRenderable_3D &orig);
        virtual ~CRenderable_3D();

        static void initComponent();
        void initializeComponentData();

        virtual gg::EMessageStatus processMessage();

    private:
        Model _3DModel;
};

#endif
