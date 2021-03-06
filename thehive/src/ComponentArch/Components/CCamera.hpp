#ifndef _CCAMERA_H
#define _CCAMERA_H

#include <ComponentArch/IComponent.hpp>
#include <ComponentArch/Message.hpp>
#include <cstdint>
#include <cmath>
#include <Omicron/Omicron.hpp>

#include <Singleton.hpp>

#include "CTransform.hpp"
#include "CRigidBody.hpp"
#include <ComponentArch/ObjectManager.hpp>

class ggDynWorld;
class CCamera : public IComponent {
friend class Factory;
public:
    CCamera(int8_t);
    CCamera(const CCamera &orig) = delete;
    virtual ~CCamera ();

    // Functions of IComponent
    virtual void Init();

    glm::vec3 getTargetPosition();
    void resetMouse();
    void setTarget(CTransform* T);
    void setExcludingBodyA(CRigidBody* R);
    void setExcludingBodyB(CRigidBody* R);
    void CameraUpdate();
    void ToggleCameraInversionY();

    glm::vec3 getCameraPosition();
    glm::vec3 getOffsetPositionVector();

    void moveCameraPosition(glm::vec3);

    void getDirectionVector(glm::vec3 &Output);

    void ToogleFreeCamera();
    void FollowTarget();
    void FreeCamera();

private:
    Omicron         *Engine;
    StandardNode    *cam;
    CTransform      *Target;
    ggDynWorld      *dynWorld;
    CRigidBody      *collider;

    CRigidBody      *ExcludingBodyA;
    CRigidBody      *ExcludingBodyB;


    glm::vec3 pos_on_collision;
    glm::vec3 CameraTarget;
    glm::vec3 CurrentPosition;
    glm::vec3 LastFreeCameraPosition;

    int8_t InvertCamera;
    bool collision;

    void fixCameraPositionOnCollision(const glm::vec3 &Target, const glm::vec3 &CameraPosition);
    void setPerpendicularOffsetVector(glm::vec3&);

    double prevX, prevY;
    float t, p;

    using FunctionPTR = void (CCamera::*)();
    FunctionPTR CurrentUpdate;
};


#endif
