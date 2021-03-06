#include "CGun.hpp"
#include <Omicron/2D/Motor2D.hpp>
#include <Bullet/ggDynWorld.hpp>
#include "CAIEnem.hpp"
#include <ComponentArch/Components/CVida.hpp>

#define FORCE_FACTOR        1000.f
#define DIST_OFFSET         2.f

CGun::CGun(float _dmg, float _cadence, int _charger_bullets, int _total_bullets, float _reloadDT, float _range, int _wType, std::string sonido_disparo, std::string sonido_recarga, std::string sonido_desenfundado, std::string sonido_vacio)
:Manager(nullptr), cTransform(nullptr),
damage(_dmg), cadence(_cadence), charger_bullets(_charger_bullets), total_bullets(_total_bullets),
reloadDT(_reloadDT), range(_range), WEAPON_TYPE(_wType)
{
    kcharger_bullets = charger_bullets;
    ktotal_bullets = total_bullets;
    canShoot = true;
    reloading = false;
    generatorDT = 2;

    SS = Singleton<SoundSystem>::Instance();
    hud = Singleton<HUD>::Instance();

    s_disparo = new SonidoNormal();
    SS->createSound(sonido_disparo, s_disparo);
    s_desenfundado = new SonidoNormal();
    SS->createSound(sonido_desenfundado, s_desenfundado);
    s_vacio = new SonidoNormal();
    SS->createSound(sonido_vacio, s_vacio);

    if(_wType != 1){
        s_recarga = new SonidoNormal();
    }
    else{
        s_recarga = new SonidoEscopeta();
    }
    SS->createSound(sonido_recarga, s_recarga);

    s_cogida = new SonidoNormal();
    SS->createSound("event:/SFX/Objetos/Arma", s_cogida);

}

CGun::~CGun() {

    delete s_disparo;
    delete s_desenfundado;
    delete s_vacio;
    delete s_recarga;
    delete s_cogida;

}
void CGun::reset(){
    charger_bullets=kcharger_bullets ;
    total_bullets=ktotal_bullets ;
    canShoot = true;
    reloading = false;
    generatorDT = 2;

}
void CGun::shoot(glm::vec3 to){
    CTriggerSystem* EventSystem=Singleton<CTriggerSystem>::Instance();
    if(canShoot && !reloading){
        // Activar cadencia
        canShoot = false;
        dtCadence = std::chrono::high_resolution_clock::now();

        // Comprobar balas
        if(!charger_bullets){
            //gg::cout("Click!");
            s_vacio->play();
            //EventSystem->PulsoTrigger(kTrig_EnemyNear,0,cTransform->getPosition(),500,TData());

            return;
        }

        s_disparo->play();
        // Comprobar si no es la katana
        if(total_bullets!=-1){
            charger_bullets--;
            if(WEAPON_TYPE == 1)
                s_recarga->setParameter("Lleno",0);
        }

        // Singleton<Motor2D>::Instance()->setbullet(0,charger_bullets,total_bullets);
        // HUD hud;
        // hud.setPrimaryBullets(charger_bullets);
        // hud.setPrimaryChamber(total_bullets);

        // Comprobar destino
        if(to.x == -1){
            //gg::cout("PAM! - "+std::to_string(total_bullets));
            return;
        }

        //gg::cout("PIM! - "+std::to_string(total_bullets));

        //TData mes;
        //CTriggerSystem* EventSystem=Singleton<CTriggerSystem>::Instance();
        //EventSystem->PulsoTrigger(kTrig_EnemyNear,0,cTransform->getPosition(),500,TData());

        // // //std::cout << "PIM!!! -> " << total_bullets << '\n';
        glm::vec3 from = cTransform->getPosition();

        glm::vec3 vel=glm::normalize(to-from);
        // Se modulara segun el danyo de cada arma 0-1
        vel *= FORCE_FACTOR*damage;


        Singleton<ggDynWorld>::Instance()->applyForceToRaycastCollisionBody(vel);
        float id =Singleton<ggDynWorld>::Instance()->getIDFromRaycast();
        //CTriggerSystem* EventSystem=Singleton<CTriggerSystem>::Instance();
        //EventSystem->PulsoTrigger(kTrig_Shoot,0,to,500,mes);//sonido de disparo
        /////////cambiar todo esto a una funcion de CAIENEM

        if(id!=-1){
            CAIEnem* AIEnem = static_cast<CAIEnem*>(Manager->getComponent(gg::AIENEM,id));
            if(AIEnem){
                AIEnem->setPlayerIsAttacking(true);
                //
                CVida *health = static_cast<CVida*>(Manager->getComponent(gg::VIDA,id));
                if(health){
                    //gg::cout("PUM! -> ["+std::to_string(damage)+"]", gg::Color(0, 0, 255, 1));
                    health->quitarvida(damage);
                }
            }
        }

        // <DEBUG>
            // Factory *fac = Singleton<Factory>::Instance();
            // fac->createDebugBullet(glm::vec3(to));
        // </DEBUG>
    }
}

void CGun::reload(){
    // NEED TO APPLY THE RELOAD TIME
    //gg::cout(" -- RELOAD -- ");
    reloading = true;
    dtReload = std::chrono::high_resolution_clock::now();

    if(WEAPON_TYPE!=1)
        s_recarga->play();
    else
        recarga_escopeta();

}

void CGun::recarga_escopeta(){
    if(charger_bullets != kcharger_bullets-1){
        s_recarga->play();
    }
    else{
        s_recarga->setParameter("Lleno",1);
        s_recarga->play();
    }
}

bool CGun::isReloading(){
    return reloading;
}

int CGun::getBullets(){
    return charger_bullets;
}

int CGun::getTotalBullets(){
    return total_bullets;
}

int CGun::getType(){
    return WEAPON_TYPE;
}

bool CGun::canReload(){
    if(kcharger_bullets > charger_bullets && total_bullets > 0)    return true;
    else                                return false;
}

void CGun::Init(){
    // Inicializar singletons
    Manager = Singleton<ObjectManager>::Instance();

    //  Inicializar punteros a otras compnentes
    MHandler_SETPTRS();
}


gg::EMessageStatus CGun::processMessage(const Message &m) {

    if (m.mType == gg::M_SETPTRS)  return MHandler_SETPTRS ();

    return gg::ST_ERROR;
}


//  Message handler functions_______________________________________________________________
//|     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |

gg::EMessageStatus CGun::MHandler_SETPTRS(){
    // Inicializando punteros

    cTransform = static_cast<CTransform*>(Singleton<ObjectManager>::Instance()->getComponent(gg::TRANSFORM, getEntityID()));
    //Singleton<Motor2D>::Instance()->setbullet(0,total_bullets,ktotal_bullets);

    return gg::ST_TRUE;
}

void CGun::FixedUpdate(){
    // UPDATE
    if(reloading){
        auto end         = std::chrono::high_resolution_clock::now();
        auto elapsedtime = end - dtReload;
        auto ms          = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedtime).count();

        if(ms > reloadDT*1000){
            //gg::cout(" -- RELOADED" , gg::Color(255, 0, 0, 1));
            reloading = false;
            if(WEAPON_TYPE == 1){
                if(total_bullets>0){
                    charger_bullets++;
                    total_bullets--;
                }
            }
            else{
                if(total_bullets-(kcharger_bullets-charger_bullets)<0){
                    charger_bullets = charger_bullets + total_bullets;
                    total_bullets=0;
                }
                else{
                    total_bullets = total_bullets - (kcharger_bullets-charger_bullets);
                    charger_bullets = kcharger_bullets;
                }
            }
        }
    }
    else if(!canShoot){
        auto end         = std::chrono::high_resolution_clock::now();
        auto elapsedtime = end - dtCadence;
        auto ms          = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedtime).count();

        if(ms > (1/cadence)*1000){
            // //gg::cout(" --- READY TO SHOOT --- ", gg::Color(255, 0, 0, 1)),
            canShoot = true;
        }
    }
    fullDeBalas(0);
}

void CGun::fullDeBalas(int a){
    auto end         = std::chrono::high_resolution_clock::now();
    auto elapsedtime = end - dtBulletGenerator;
    auto ms          = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedtime).count();
    if(ms > generatorDT*1000){
        if(total_bullets<ktotal_bullets){
            total_bullets++;
        }
        dtBulletGenerator = std::chrono::high_resolution_clock::now();
    }

    // Singleton<Motor2D>::Instance()->setbullet(a,charger_bullets,total_bullets);
    if(a == 0){
        // Primaria
        hud->setPrimaryBullets(charger_bullets);
        hud->setPrimaryChamber(total_bullets);
    }
    else{
        // Secundaria
        hud->setSecondaryBullets(charger_bullets);
        hud->setSecondaryChamber(total_bullets);
    }
}


void CGun::desenfundado(){
    s_desenfundado->play();
}
void CGun::cogida(){
    s_cogida->play();
}
