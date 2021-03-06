#include "OptionState.hpp"

//#include <iostream>
#include <cstdint>
#include <string>
#include <stack>

#include "ComponentArch/ObjectManager.hpp"
#include "Singleton.hpp"
#include <Omicron/2D/Motor2D.hpp>
#include <States/StateMachine.hpp>



#define MOVEMENT_SPEED 1.f


//Funciones de Fran Gallego para imprimir memoria por consola ==============================¬
//                                                                                          |
//====================================================================================      |
// Pretty print a 2-digits hexadecimal value                                                |
//====================================================================================      |
/*
void printHexVal(uint16_t val) {
   // //std::cout << std::hex << std::setw(2) << std::setfill('0') << val;
}

//====================================================================================
// Print a memory slice as raw bytes
//====================================================================================
void printRawMem(uint8_t* p, uint16_t linebytes, uint16_t lines) {
   for(uint16_t l=0; l < lines; ++l) {
      // //std::cout << reinterpret_cast<uint16_t*>(p) << " ";
      for(uint16_t u=0; u < linebytes; ++u) {
         printHexVal(*p);
         ++p;
         // //std::cout << " ";
      }
      // //std::cout << "\n";
   }
}
*/
//============================================================================================

OptionState::OptionState()
:BACKGROUND_TEXTURE_ID(Singleton<AssetManager>::Instance()->getTextureWithoutSavingToMap("assets/HUD/OPCIONES.png")),
 BACKGROUND(0,0,1,1, BACKGROUND_TEXTURE_ID)
{
    cont = Singleton<GUIController>::Instance();
    Engine = Singleton<Omicron>::Instance();
    SS = Singleton<SoundSystem>::Instance();
    Engine->HideCursor(false);
    BACKGROUND.setZindex(-0.995f);
}

OptionState::~OptionState(){
    CLIN();
}

void OptionState::Init(){
    Singleton<Motor2D>::Instance()->InitOptions();
    //Engine->createCamera(glm::vec3(0, 30, 30), glm::vec3(0, 0, 0));
}

void OptionState::Resume() {
    Init();
}

void OptionState::Update(){
    Engine->PollEvents();

    Engine->BeginDraw();
    Engine->draw();
    cont->update();
    //Singleton<Motor2D>::Instance()->DisplayMenu();
    //Singleton<StateMachine>::Instance()->AddState(new GameState());

    BACKGROUND.Draw();
    Singleton<Motor2D>::Instance()->draw();
    Singleton<Motor2D>::Instance()->checkbuton();
    Singleton<Motor2D>::Instance()->aplyhover();
    //Engine2D->draw();
    //Engine2D->checkbuton();

    Engine->EndDraw();
    Engine->resetClickVariable();

    SS->update();

}

void OptionState::CLIN(){

    Singleton<Motor2D>::Instance()->CLINMenu();
    Singleton<AssetManager>::Instance()->freeTexture(BACKGROUND_TEXTURE_ID);

    //Blackboard::ClearGlobalBlackboard();
    //Manager->clin();
    //world->clean();
}
