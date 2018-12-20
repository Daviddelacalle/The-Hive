#ifndef _CCAMERA_H
#define _CCAMERA_H

#include <ComponentArch/IComponent.hpp>
#include <ComponentArch/Message.hpp>
#include <cstdint>
#include <cmath>
#include <GameEngine/GameEngine.hpp>
#include <GameEngine/Camera.hpp>
#include <Singleton.hpp>
#include <Bullet/ggDynWorld.hpp>

#include "CTransform.hpp"

class ObjectManager;
class CRigidBody;

class CCamera : public IComponent {
friend class Factory;
public:
    CCamera(bool);
    CCamera(const CCamera &orig) = delete;
    virtual ~CCamera ();

    // Functions of IComponent
    virtual void Init();

    void setTarget(CTransform* T);
    void CameraUpdate();

    gg::Vector3f getCameraPosition();
    gg::Vector3f getCameraRotation();
    gg::Vector3f getCameraTarget();
    gg::Vector3f getOffsetPositionVector();

    void moveCameraPosition(gg::Vector3f);

    gg::Vector3f getlastHeroPosition();
    gg::Vector3f getCameraPositionBeforeLockRotation();
    void setCameraPositionBeforeLockRotation(gg::Vector3f); // Por ahora no se usa

    inline void SwitchInvertCamera(){ InvertCamera = !InvertCamera;};

private:
    GameEngine      *Engine;
    ObjectManager   *Manager;
    Camera          *cam;
    CTransform      *Target;
    ggDynWorld      *dynWorld;

    gg::Vector3f lastHeroPosition;
    gg::Vector3f cameraPositionBeforeLockRotation;
    gg::Vector3f offsetPositionVector;
    gg::Vector3f pos_on_collision;
    gg::Vector3f last_cam_position;

    float screenW;
    float screenH;

    bool InvertCamera;
    bool collision;

    void getNewRotation                 (gg::Vector3f&);
    void setFinalRotation               (gg::Vector3f&,gg::Vector3f&,bool);
    void setHorizontalAxis              (gg::Vector3f&,gg::Vector3f&,gg::Vector3f&,gg::Vector3f&);
    void setVerticalAxis                (gg::Vector3f&,gg::Vector3f&,gg::Vector3f&, gg::Vector3f&);
    void fixCameraPositionOnCollision   (gg::Vector3f&);
    void setPerpendicularOffsetVector   (gg::Vector3f&);

};


#endif
