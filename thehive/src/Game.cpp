#include "Game.hpp"

#include <iostream>
#include <cstdint>
#include <string>
#include <stack>

#include "ComponentArch/ObjectManager.hpp"
#include "GameEngine/Camera.hpp"
#include "Singleton.hpp"
#include "GameAI/Pathfinding.hpp"
#include "GameAI/AIDirector.hpp"
#include "GameAI/NavmeshStructs.hpp"

#include "GameEngine/ScreenConsole.hpp"

#include "Factory.hpp"
#include <ComponentArch/Components/CNavmeshAgent.hpp>
#include <EventSystem/Blackboard.hpp>
#include "BinaryParser.hpp"
#include <SurrealEngine/SurrealEngine.hpp>

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

Game::Game()
:Accumulator(0)
{
    Engine = Singleton<SurrealEngine>::Instance();
    EventSystem = Singleton<CTriggerSystem>::Instance();
    Director = Singleton<AIDirector>::Instance();
    //Director = new AIDirector();

    //Engine->Starto();
    //Engine->HideCursor(true);

    Manager = Singleton<ObjectManager>::Instance();

    world = Singleton<ggDynWorld>::Instance();
    //world->inito();
    //Engine->HideCursor(true);
}

Game::~Game(){

}

void Game::Init(){
    //Singleton<ScreenConsole>::Instance()->InitHUD();
    BinaryParser::test();
    auto sF = Singleton<Factory>::Instance();
    Engine->crearCamara(90,0.1f,100.f, glm::vec3(2,2,10),glm::vec3(),16.f/9.f);
    Engine->crearLuz(col,glm::vec3(5, 6, 0),glm::vec3(), AssetManager::getShader("Default"));
    Engine->print();
    // Pos init del heroe normal
    // 360, 0, 350


    uint16_t h = sF->createHero(glm::vec3(0,6,0),1);
    //sF->createRusher(glm::vec3(0, 6, 0), 10);
    //sF->createRusher(glm::vec3(5,3,65),200);
    Director->init();

    MainCamera = static_cast<CCamera*>(Manager->getComponent(gg::CAMERA, h));

    // auto cam = Engine->getCamera();
    //
    // Engine->setPosition(cam, glm::vec3(2,10,10));

    //static_cast<TCamara*>(cam->getEntidad())->setTarget(glm::vec3(0,0,0));

    Accumulator = 0;
    // Material yelo("assets/Models/CIUDAD/Presentacion1/NAVMESH.png");
    //
    // uint16_t Navmesh = Manager->createEntity();
    // CRenderable_3D* Renderable_3D = new CRenderable_3D("assets/Models/CIUDAD/Presentacion1/NAVMESH.obj", yelo);
    // Manager->addComponentToEntity(Renderable_3D, gg::RENDERABLE_3D, Navmesh);

    Singleton<Pathfinding>::Instance()->SetDebug(false);
    world->setDebug(true);
    MasterClock.Restart();

    // std::cout << "\n -- INIT -- " << '\n';
}

void Game::Update(){
    //CTransform* cTransform2 = static_cast<CTransform*>(Manager->getComponent(gg::TRANSFORM,Manager->getHeroID()));
    //std::cout << "POS BUENA:" <<cTransform2->getPosition()<< '\n';

    DeltaTime = MasterClock.Restart().Seconds();

    // std::cout << " - POLL EVENTS" << '\n';
    Engine->PollEvents();


    if(DeltaTime > 0.25) DeltaTime = 0.25;

    // std::cout << " - WHILE" << '\n';
    Accumulator += DeltaTime;

    // std::cout << "  - Delta time1 : " << DeltaTime << '\n';
    // if(DeltaTime > 10000) throw std::exception();
    while(Accumulator >= 1/UPDATE_STEP){
        // FIXED UPDATE//
        Manager->sendMessageToAllEntities(Message(gg::M_INTERPOLATE_PRESAVE));
        Manager->FixedUpdateAll();
        Manager->sendMessageToAllEntities(Message(gg::M_INTERPOLATE_POSTSAVE));
        // Director->comprobar();
        // Director->clipingEnemigos();
        world->stepSimulation(1/UPDATE_STEP*2.5, 10);
        Accumulator -= 1/UPDATE_STEP;
    }

    // std::cout << " - EVENTSYSTEM UPDATE" << '\n';
    EventSystem->Update();
    //Director->update(DeltaTime);
    //Director->clipingEnemigos();
    //  Interpolation tick!
    Tick = std::min(1.f, static_cast<float>( Accumulator/(1/UPDATE_STEP) ));
    Manager->sendMessageToAllEntities(Message(gg::M_INTERPOLATE, &Tick));

    // std::cout << " - BEGIN DRAW" << '\n';
    Engine->BeginDraw();

    // std::cout << "  - UPDATE ALL" << '\n';
    Manager->UpdateAll();

    //std::cout << "Update" << '\n';
    MainCamera->CameraUpdate();
    //Engine->print();

    // std::cout << "  - DRAW" << '\n';
    Engine->draw();
    Engine->DisplayFPS();

    // Consola por pantalla
    // Singleton<ScreenConsole>::Instance()->DisplayDebug();
    // Singleton<ScreenConsole>::Instance()->DisplayHUD();
    Singleton<ggDynWorld>::Instance()->debugDrawWorld();
    // Singleton<Pathfinding>::Instance()->DroNodes();

    // std::cout << " - END DRAW" << '\n';
    Engine->EndDraw();
}

void Game::Resume(){
    //Engine->HideCursor(true);
}

void Game::CLIN(){
    //Blackboard::ClearGlobalBlackboard();
    Manager->clin(); // este esta bien creo
    world->clear();  //clean es el vuestro// clear solo vacia los rigidbody sin quitar las fisicas
    //EventSystem->clin();
    //Singleton<ScreenConsole>::Instance()->CLIN();
    //Singleton<ScreenConsole>::Instance()->CLINNormal();

    // std::cout << "1/60 = " << 1/60.F << '\n';
    // std::cout << "UPDTES " << UPDATE  << '\n';
    // std::cout << "DRO " << DRO  << '\n';

}
