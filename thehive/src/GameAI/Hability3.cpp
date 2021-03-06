#include "Hability3.hpp"
//#include "Enum.hpp"
#include <EventSystem/TData.hpp>
#include <EventSystem/CTriggerSystem.hpp>
#include <ComponentArch/Components/CTransform.hpp>
#include <ComponentArch/Components/CPlayerController.hpp>

#include <Singleton.hpp>

//ultrasonido

    /* data */
    //void Hability::iniciar (Blackboard* _data){
    //    Hability (Blackboard* _data);

    //}
    Hability3::Hability3 (int _id):Hability(_id,0,4000)
    {
        SS = Singleton<SoundSystem>::Instance();

        s_hab = new SonidoNormal();
        SS->createSound("event:/SFX/Jugador/Habilidades/ExpansiveWave", s_hab);
    }
    Hability3::Hability3 (){

    }
    Hability3::~Hability3 (){
        delete s_hab;
    }

    Hability3::Hability3 (const Hability3 &orig){


    id=orig.id;
    duracion=orig.duracion;
    cooldown=orig.cooldown;
    nocool=orig.nocool;
    terminado=orig.terminado;
    nTimeStamp=orig.nTimeStamp;
    cTriggerSystem=orig.cTriggerSystem;




}


    //codigo de las habilidades
    void Hability3::Hab1_init (){
        TData mes;
        mes.add(kDat_Damage,3000);
        CTriggerSystem* EventSystem=Singleton<CTriggerSystem>::Instance();

        CTransform* cTransform = static_cast<CTransform*>(Singleton<ObjectManager>::Instance()->getComponent(gg::TRANSFORM, id));
        float id=cTransform->getEntityID();
        EventSystem->PulsoTrigger(kTrig_ExpansiveForce,id,cTransform->getPosition(),100,mes);
        //EventSystem->RegisterTriger(kTrig_Explosion,1,0,cTransform->getPosition(), radius, 10000,false,mes);

        s_hab->play();

    }
