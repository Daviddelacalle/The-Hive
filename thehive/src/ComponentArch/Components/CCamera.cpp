#include "CCamera.hpp"
#include <ComponentArch/ObjectManager.hpp>
#include <Bullet/ggDynWorld.hpp>

#define PI                  3.14159265358979323846264338
#define DEGREES_TO_RADIANS  PI/180.f
#define CAMERA_ATENUATION   7.f
#define HEIGHT              0.4
#define RADIUS              2.5

CCamera::CCamera(bool _b)
:Target(nullptr), Engine(nullptr), cam(nullptr),
InvertCamera(_b)
{}


CCamera::~CCamera(){
}

void CCamera::Init(){
    Engine = Singleton<TMotorTAG>::Instance();
    dynWorld = Singleton<ggDynWorld>::Instance();
    cam = Engine->getCamera();

    collision = false;


    screenW = Engine->getScreenWidth();
    screenH = Engine->getScreenHeight();

    Engine->getCursorPosition(prevX, prevY);
    t = 0;
    p = 0;
}

void CCamera::setTarget(CTransform *T) {
    Target = T;
}

void CCamera::CameraUpdate(){
    double x, y;
    Engine->getCursorPosition(x, y);
    t += (prevX - x) * 0.005f;
    p += (y - prevY) * 0.005f * InvertCamera;

    if(t < 0) t = 2*PI;
    else if(t > 2*PI) t = 0;

    if(p < -PI/2+0.2) p = -PI/2+0.2;
    else if(p > PI/2-0.2) p = PI/2-0.2;

    prevX = x;
    prevY = y;

    glm::vec3 _target = Target->getPosition();

    CurrentPosition.x = _target.x + 5 * sin(t)*cos(p);
    CurrentPosition.y = _target.y + 5 * sin(p);
    CurrentPosition.z = _target.z + 5 * cos(t)*cos(p);

    Engine->setPosition(cam, CurrentPosition);
    static_cast<TCamara*>(cam->getEntidad())->setTarget(_target);
}


void CCamera::fixCameraPositionOnCollision(glm::vec3 &nextPosition){
    glm::vec3 camPosition = CurrentPosition;
    // Las dos mejores lineas que he escrito en mi vida
    glm::vec3 FIXED_NEXT_POSITION = nextPosition+(camPosition-nextPosition)*0.2f;
    if(dynWorld->RayCastTest(FIXED_NEXT_POSITION,camPosition,pos_on_collision)){
        Engine->setPosition(cam, pos_on_collision);
        CurrentPosition = pos_on_collision;
    }
}

void CCamera::getDirectionVector(glm::vec3 &Output){
    Output = CurrentPosition - Target->getPosition();
    Output.y = 0;
}

glm::vec3 CCamera::getCameraPosition(){
    return CurrentPosition;
}

glm::vec3 CCamera::getCameraRotation(){
    return Rot;
}


void CCamera::moveCameraPosition(glm::vec3 _offPos){
    CurrentPosition += _offPos;
    Engine->setPosition(cam, CurrentPosition);
}
