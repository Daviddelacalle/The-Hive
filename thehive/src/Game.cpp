#include "Game.hpp"

#include <iostream>
#include <cstdint>
#include <string>
#include <stack>

#include "ComponentArch/ObjectManager.hpp"
#include "GameEngine/Camera.hpp"
#include "Singleton.hpp"
#include "GameAI/Pathfinding.hpp"
#include "GameAI/NavmeshStructs.hpp"

#include "GameEngine/ScreenConsole.hpp"

#include "Factory.hpp"
#include <ComponentArch/Components/CNavmeshAgent.hpp>
#include <EventSystem/Blackboard.hpp>

#define MOVEMENT_SPEED 1.f

#define FRAMERATE 60.f
#define UPDATE_STEP 15.f
#define BULLET_STEP 1.f/FRAMERATE

//Funciones de Fran Gallego para imprimir memoria por consola ==============================¬
//                                                                                          |
//====================================================================================      |
// Pretty print a 2-digits hexadecimal value                                                |
//====================================================================================      |
/*
void printHexVal(uint16_t val) {
}

//====================================================================================
// Print a memory slice as raw bytes
//====================================================================================
void printRawMem(uint8_t* p, uint16_t linebytes, uint16_t lines) {
   for(uint16_t l=0; l < lines; ++l) {
      for(uint16_t u=0; u < linebytes; ++u) {
         printHexVal(*p);
         ++p;
      }
   }
}
*/
//============================================================================================

Game::Game(){
    Engine = Singleton<GameEngine>::Instance();
    EventSystem = Singleton<CTriggerSystem>::Instance();

    //Engine->Starto();
    //Engine->HideCursor(true);

    Manager = Singleton<ObjectManager>::Instance();

    world = Singleton<ggDynWorld>::Instance();
    //world->inito();
    Engine->HideCursor(true);
    UPDATE = 0;
    DRO = 0;
}

Game::~Game(){

}

void Game::Init(){
    Singleton<ScreenConsole>::Instance()->InitHUD();

    auto sF = Singleton<Factory>::Instance();
    uint16_t key;

    Engine->createCamera(gg::Vector3f(0, 30, 30), gg::Vector3f(0, 0, 0));


    uint16_t h = sF->createHero(gg::Vector3f(360, 0, 350),false);     //600
    MainCamera = static_cast<CCamera*>(Manager->getComponent(gg::CAMERA, h));
    sF->createSoldier(gg::Vector3f(360, -10, 390),1000);
    sF->createSoldier(gg::Vector3f(397, 0, 350),2000);

    sF->createCollectableWeapon(gg::Vector3f(397, 0, 330),2);

    gg::Vector3f mapPos(0,0,0);
    sF->createCollisionableStaticModel(
        "assets/Models/CIUDAD/PROTOTIPO5/PROTOTIPO_CIUDAD.obj",
        "assets/Models/CIUDAD/PROTOTIPO5/PROTOTIPO_CIUDAD.bullet",
        "assets/Models/CIUDAD/PROTOTIPO5/PROTOTIPO_CIUDAD.png",
        mapPos
    );

    sF->createCollectableWeapon(gg::Vector3f(-700, -20, 20),0);
    sF->createCollectableWeapon(gg::Vector3f(-700, -20, 40),1);
    sF->createCollectableWeapon(gg::Vector3f(-700, -20, 60),2);
    sF->createCollectableWeapon(gg::Vector3f(-700, -20, 80),3);
    sF->createCollectableWeapon(gg::Vector3f(-700, -20, 100),4);

    sF->createCollisionableDynamicModel(
        "assets/Models/cuboGrande.obj",
        "assets/BoundingBoxes/cuboGrande.bullet",
        "assets/Textures/e61.png",
        gg::Vector3f(mapPos.X+274, mapPos.Y, mapPos.Z+50));

    uint16_t idEx = sF->createCollisionableDynamicModel(
        "assets/Models/ModelsForEvents/door1.obj",
        "assets/Models/ModelsForEvents/door1.bullet",
        "assets/Textures/Domino.jpg",
        gg::Vector3f(mapPos.X+350, mapPos.Y-10, mapPos.Z+204));

    key = sF->createPickableItem(gg::Vector3f(224, 0, 145));
    sF->createTouchableObject(gg::Vector3f(mapPos.X+345, mapPos.Y-13, mapPos.Z+215),idEx,gg::Vector3f(0,0.4,0),3200);

    idEx = sF->createCollisionableDynamicModel(
        "assets/Models/ModelsForEvents/door2.obj",
        "assets/Models/ModelsForEvents/door2.bullet",
        "assets/Textures/Domino.jpg",
        gg::Vector3f(mapPos.X+270.5, mapPos.Y-8, mapPos.Z+131));
        // i674,106,130
    sF->createTouchableObject(gg::Vector3f(mapPos.X+290, mapPos.Y-11, mapPos.Z+131),idEx,gg::Vector3f(0,-0.035,0),8000,key);

    ////////////////////////////////////////////////////////////////
    // TABLA MOLONA DE CONVERSION DE LA POSICION EN EL ESPACIO 3D //
    ////////////////////////////////////////////////////////////////
    //              X        Y       Z                            //
    // ---------------------------------------------------------- //
    // BELNDER  -> bX,      bY,     bZ                            //
    // JEUGO    -> jX-bX,   jY+bZ,  jZ-bY                         //
    ////////////////////////////////////////////////////////////////


    //factory->createCollisionableStaticModel("assets/Models/CIUDAD/PROTOTIPO_CIUDAD.obj", "assets/Models/CIUDAD/PROTOTIPO_CIUDAD.bullet", "assets/Models/CIUDAD/PROTOTIPO_CIUDAD.png", gg::Vector3f(700, 175, 0));
    // uint16_t Dummy = sF->createPathFindingActor(gg::Vector3f(285, 0, 117));
    // //factory->createStaticModel("assets/NavMeshes/L4D2Nav.obj", "assets/NavMeshes/L4D2Nav.png", gg::Vector3f(0, 0, 0));
    //
    // CNavmeshAgent* Agent = static_cast<CNavmeshAgent*>(Manager->getComponent(gg::NAVMESHAGENT, Dummy));
    // Agent->SetDestination(gg::Vector3f(280, -20, -377));
    //
    // uint16_t Navmesh = Manager->createEntity();
    // Material yelo("assets/NavMeshes/PROTOTIPO_CIUDAD.png");
    // //Material yelo("assets/Textures/ice.bmp");
    //
    // CTransform* Transform = new CTransform(gg::Vector3f(0,80,0), gg::Vector3f(0,0,0));
    // Manager->addComponentToEntity(Transform, gg::TRANSFORM, Navmesh);
    //
    // CRenderable_3D* Renderable_3D = new CRenderable_3D("assets/NavMeshes/PROTOTIPO_CIUDAD.obj", yelo);
    // Manager->addComponentToEntity(Renderable_3D, gg::RENDERABLE_3D, Navmesh);

    // uint16_t NavmeshGround = Manager->createEntity();
    // CRigidBody* RigidBody = new CRigidBody(false, false,"", 0, -10, 0, 680,10,800, 0, 0,0,0, 0.2);
    // Manager->addComponentToEntity(RigidBody, gg::RIGID_BODY, NavmeshGround);

    // std::cout << "BEGIN GAME LOOP" << '\n';
    gg::cout("Testing", gg::Color(255, 0, 0, 1));

    Singleton<Pathfinding>::Instance()->SetDebug(false);

    MasterClock.Restart();


        //  SPIRAL OF DEATH. Not cool


    //while(Engine->isWindowOpen()) {
    //    update();

    //}

}
void Game::Update(){
    DeltaTime = MasterClock.Restart().Seconds();

    if(DeltaTime > 0.25) DeltaTime = 0.25;

    Accumulator += DeltaTime;
    while(Accumulator >= 1/UPDATE_STEP){
        // FIXED UPDATE
        Manager->sendMessageToAllEntities(Message(gg::M_INTERPOLATE_PRESAVE));
        Manager->FixedUpdateAll();
        Manager->sendMessageToAllEntities(Message(gg::M_INTERPOLATE_POSTSAVE));
        world->stepSimulation(1/UPDATE_STEP*2.5, 10);
        Accumulator -= 1/UPDATE_STEP;
        ++UPDATE;
    }
    ++DRO;

    EventSystem->Update();

    //  Interpolation tick!
    Tick = std::min(1.f, static_cast<float>( Accumulator/(1/UPDATE_STEP) ));
    Manager->sendMessageToAllEntities(Message(gg::M_INTERPOLATE, &Tick));



    Engine->BeginDro();
    Manager->UpdateAll();

    MainCamera->CameraUpdate();

    Engine->Dro();
    Engine->DisplayFPS();

    Singleton<ggDynWorld>::Instance()->debugDrawWorld();
    Singleton<Pathfinding>::Instance()->DroNodes();
    Singleton<ScreenConsole>::Instance()->DisplayDebug();
    Singleton<ScreenConsole>::Instance()->DisplayHUD();

    Engine->EndDro();
}

void Game::Resume(){

}

void Game::CLIN(){
    //Blackboard::ClearGlobalBlackboard();
    Manager->clin();// este esta bien creo
    world->clear();//clean es el vuestro// clear solo vacia los rigidbody sin quitar las fisicas
    //EventSystem->clin();
    //Singleton<ScreenConsole>::Instance()->CLIN();
    Singleton<ScreenConsole>::Instance()->CLINNormal();

    std::cout << "1/60 = " << 1/60.F << '\n';
    std::cout << "UPDTES " << UPDATE  << '\n';
    std::cout << "DRO " << DRO  << '\n';

}
