#include "GUIController.hpp"
#include <iostream>
#include <Game.hpp>
#include <OptionState.hpp>
//#include <SMaterial>
GUIController::GUIController()
:Engine(nullptr)
{
  Init();
}
void GUIController::setposmax(int p){
    cursorpos=0;
    posmax=p;
}
void GUIController::Init(){
    dif=1;
    dialogue=50;
    music=50;
    effect=50;
    cursorpos=0;
    music_max=100;
    effect_max=100;
    dialogue_max=100;
    posmax=0;
    arriba_pulsado=false;
    bajo_pulsado=false;
    enter_pulsado=false;
    back_pulsado=false;
    esc_pulsado=false;
    Engine = Singleton<GameEngine>::Instance();
    VectorAcciones[GOPLAY] = &GUIController::gotoPlay;
    VectorAcciones[GOCREDITS] = &GUIController::gotoCredits;
    VectorAcciones[GOOPTIONS] = &GUIController::gotoOptions;
    VectorAcciones[CLOSE] = &GUIController::Close;
    VectorAcciones[GOVIDEO] = &GUIController::gotoVideo;
    VectorAcciones[GOMUSIC] = &GUIController::gotoMusic;
    VectorAcciones[GOCONTROLLS] = &GUIController::gotoControlls;
    VectorAcciones[START] = &GUIController::StartGame;
    VectorAcciones[GOMAIN] = &GUIController::gotoMain;
    VectorAcciones[DIFF1] = &GUIController::dif1;
    VectorAcciones[DIFF2] = &GUIController::dif2;
    VectorAcciones[DIFF3] = &GUIController::dif3;
    VectorAcciones[CONTINUE] = &GUIController::Continue;
    VectorAcciones[RETURNMENU] = &GUIController::ReturnMain;
    VectorAcciones[GOPAUSE] = &GUIController::gotoPause;
    VectorAcciones[MOREDIALOD] = &GUIController::moreDialog;
    VectorAcciones[LESSDIALOD] = &GUIController::lessDialog;
    VectorAcciones[MOREMUSIC] = &GUIController::moreMusic;
    VectorAcciones[LESSMUSIC] = &GUIController::lessMusic;
    VectorAcciones[MOREEFFECT] = &GUIController::moreEffect;
    VectorAcciones[LESSEFFECT] = &GUIController::lessEffect;
    VectorAcciones[GOINITOPTIONS] = &GUIController::initOptions;


}
void GUIController::update(){
    //int id=-1;
    int id =Engine->checkbutton();
    if(Engine->key(gg::GG_W)){
        if(!arriba_pulsado){
            arriba_pulsado=true;
            if(cursorpos!=0){
                cursorpos--;
            }
        }
    }
    else{
        arriba_pulsado=false;
    }
    if(Engine->key(gg::GG_S)){
        if(!bajo_pulsado){
            bajo_pulsado=true;
            if(cursorpos!=(posmax-1)){
                cursorpos++;
            }
        }
    }else{
        bajo_pulsado=false;
    }
    if(Engine->key(gg::GG_Q)){
        if(!enter_pulsado){
            enter_pulsado=true;
            id=Singleton<ScreenConsole>::Instance()->Pulsarboton(cursorpos);

        }
    }else{
        enter_pulsado=false;
    }


    /////////
    //int id =Engine->checkbutton();
    if(id!=-1){
        if(VectorAcciones[id] != nullptr)
            (this->*VectorAcciones[id])();
    }
}
//but0
void GUIController::gotoPlay(){
    dif=1;
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu2());
}
//but1
void GUIController::gotoCredits(){
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu3());
}
//but2
void GUIController::gotoOptions(){
    Singleton<StateMachine>::Instance()->AddState(new OptionState(),false);
}
//but3
void GUIController::Close(){
    Singleton<GameEngine>::Instance()->Close();
}
//but4
void GUIController::gotoVideo(){
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu5());
}
//but5
void GUIController::gotoMusic(){
    Singleton<ScreenConsole>::Instance()->setVolDialogo(dialogue);
    Singleton<ScreenConsole>::Instance()->setVolMusic(music);
    Singleton<ScreenConsole>::Instance()->setVolEffect(effect);

    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu6());
}
//but6
void GUIController::gotoControlls(){
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu7());
}
//but7
void GUIController::StartGame(){
    Singleton<StateMachine>::Instance()->AddState(new Game(),false);
}
//but8
void GUIController::gotoMain(){
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu());
}
//but9
void GUIController::dif1(){
    dif=1;
}
//but10
void GUIController::dif2(){
    dif=2;
}
//but11
void GUIController::dif3(){
    dif=3;
}
//    VectorAcciones[CONTINUE] = &GUIController::dif3;
//    VectorAcciones[RETURNMENU] = &GUIController::dif3;
//but12
void GUIController::Continue(){
    Singleton<StateMachine>::Instance()->RemoveState();
}
//but13
void GUIController::ReturnMain(){
    Singleton<StateMachine>::Instance()->RemoveState(2);
}
//but 14
void GUIController::initOptions(){
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu4());
}
//but 14
void GUIController::gotoPause(){
    setposmax(Singleton<ScreenConsole>::Instance()->InitPause());
}
//but 18
void GUIController::moreDialog(){
    if(dialogue!=dialogue_max){
        dialogue++;
    }
    Singleton<ScreenConsole>::Instance()->setVolDialogo(dialogue);
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu6());
}
//but 19
void GUIController::lessDialog(){
    if(dialogue!=0){
        dialogue--;
    }
    Singleton<ScreenConsole>::Instance()->setVolDialogo(dialogue);
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu6());
}
//but 20
void GUIController::moreMusic(){
    if(music!=music_max){
        music++;
    }
    Singleton<ScreenConsole>::Instance()->setVolMusic(music);
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu6());
}
//but 21
void GUIController::lessMusic(){
    if(music!=0){
        music--;
    }
    Singleton<ScreenConsole>::Instance()->setVolMusic(music);
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu6());
}
//but 22
void GUIController::moreEffect(){
    if(effect!=effect_max){
        effect++;
    }
    Singleton<ScreenConsole>::Instance()->setVolEffect(effect);
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu6());
}
//but 23
void GUIController::lessEffect(){
    if(effect!=0){
        effect--;
    }
    Singleton<ScreenConsole>::Instance()->setVolEffect(effect);
    setposmax(Singleton<ScreenConsole>::Instance()->InitMenu6());
}