#include "CPlayerController.hpp"

#include <States/StateMachine.hpp>
#include <PauseState.hpp>
#include <IAState.hpp>
#include <PopState.hpp>
#include "CDynamicModel.hpp"
#include <AnimationLUT.hpp>
#include <ComponentArch/Components/CCamera.hpp>
#include <ComponentArch/Components/CHabilityController.hpp>
#include <ComponentArch/Components/CRigidBody.hpp>
#include <ComponentArch/Components/CAIEnem.hpp>
#include <ComponentArch/Components/CGun.hpp>
#include <GameAI/AIDirector.hpp>


#define GRENADE_VEL_FACTOR      500.f
#define GRENADE_THROW_Y_OFFSET  3.f

#define MAX_HERO_SPEED      2

#define ROTATE_KEY          gg::LCONTROL
#define DASH_KEY            gg::LSHIFT
#define RUN_KEY             gg::H
#define JUMP_KEY            gg::SPACEBAR
#define RELOAD_KEY          gg::R
#define WEAPON_KEY          gg::Q

#define FORCE_FACTOR        480.f
#define JUMP_FORCE_FACTOR   FORCE_FACTOR*4.f
#define DASH_FORCE_FACTOR   FORCE_FACTOR/50.f

#define MULT_RUN_FACTOR     1.5
#define MULT_DASH_FACTOR    1

CPlayerController::CPlayerController()
:Engine(nullptr), Manager(nullptr), world(nullptr), cTransform(nullptr), ghostCollider(nullptr), camera(nullptr),hab(nullptr)//,hab(0,2000,4000)
,ToggleFreeCameraKey(true), FreeCamera(false), PlayerMovement(true), cV(0,0,1)
{}

CPlayerController::~CPlayerController() {
    if(secondWeapon)    delete secondWeapon;
    if(collider)        delete collider;
    delete s_dash;
    delete s_pasos;
    delete s_saltar;
    delete s_saltarVoz;
}

void CPlayerController::Init(){
    //  We check if this entity has the TRANSFORM component
    Engine  = Singleton<Omicron>::Instance();
    world   = Singleton<ggDynWorld>::Instance();
    Manager = Singleton<ObjectManager>::Instance();
    factory = Singleton<Factory>::Instance();
    hud = Singleton<HUD>::Instance();
    camera  = static_cast<CCamera*>(Manager->getComponent(gg::CAMERA, getEntityID()));
    MHandler_SETPTRS();

    pulsacion_f = false;
    debug1 = false;
    debug2 = false;
    MULT_BASE=1;
    MULT_FACTOR = 1;

    maxsoldier  = 10;
    maxrusher   = 10;
    maxtank     = 10;

    currentsoldier = 1;
    currentrusher  = 1;
    currenttank    = 1;

    // AUTO-STEPPING
    glm::vec3 c_pos = ghostCollider->getBodyPosition();
    collider = new CRigidBody(false, false,"", c_pos.x,c_pos.y,c_pos.z, 0.4,0.4,0.4, 50, 0,0,0);
    collider->deactivateGravity();
    ghostCollider->setGhostObject();

    isColliderGravitySet = true;
    GH_PREV = glm::vec3(0);

    camera->setExcludingBodyA(ghostCollider);
    camera->setExcludingBodyB(collider);
    // -----------------------------

    // El heroe siempre empezara con un arma secundaria
    // Pistola por defecto
    isPrincipal = false;
    secondWeapon = nullptr;

    actualGrenadeState = 1;
    mapFuncGrenades.insert(std::make_pair(1,&CPlayerController::playerThrowHolyBomb));
    mapFuncGrenades.insert(std::make_pair(2,&CPlayerController::playerThrowMatrioska));
    mapFuncGrenades.insert(std::make_pair(3,&CPlayerController::playerThrowDopple));
    items.fill(0);

    SS = Singleton<SoundSystem>::Instance();

    s_dash = new SonidoNormal();
    SS->createSound("event:/SFX/Jugador/Habilidades/Dash", s_dash);

    s_pasos = new SonidoSuperficie();
    SS->createSound("event:/SFX/Jugador/Pasos", s_pasos);

    s_saltar = new SonidoSuperficie();
    SS->createSound("event:/SFX/Jugador/Saltar", s_saltar);

    s_saltarVoz = new SonidoNormal();
    SS->createSound("event:/Voces/Jugador/Golpe", s_saltarVoz);

    KEYMAP[0] = {gg::_1, &CPlayerController::ToggleSkill1};
    KEYMAP[1] = {gg::_2, &CPlayerController::ToggleSkill2};
    KEYMAP[2] = {gg::_3, &CPlayerController::ToggleSkill3};
    KEYMAP[3] = {RELOAD_KEY, &CPlayerController::ReloadGun};
    KEYMAP[4] = {gg::G, &CPlayerController::ThrowGranade};
    KEYMAP[5] = {WEAPON_KEY, &CPlayerController::ChangeWeapon};
    KEYMAP[6] = {RUN_KEY, &CPlayerController::Run};
    KEYMAP[7] = {DASH_KEY, &CPlayerController::DASH};
    KEYMAP[8] = {JUMP_KEY, &CPlayerController::JUMP};
    KEYMAP[9] = {gg::P, &CPlayerController::TogglePause};

    KEYMAP[10] = {gg::_4, &CPlayerController::ToggleFreeCamera};
    KEYMAP[11] = {gg::M, &CPlayerController::EnemyInfo};

    KEYMAP[12] = {gg::V, &CPlayerController::MostrarTexto};
    // KEYMAP[15] = {gg::C, &CPlayerController::QuitarTexto};
}

gg::EMessageStatus CPlayerController::processMessage(const Message &m) {

    if (m.mType == gg::M_SETPTRS)  return MHandler_SETPTRS ();

    return gg::ST_ERROR;
}


// Message handler functions__________________________________________________________________
// |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |

gg::EMessageStatus CPlayerController::MHandler_SETPTRS(){
    cTransform = static_cast<CTransform*>(Manager->getComponent(gg::TRANSFORM, getEntityID()));
    ghostCollider = static_cast<CRigidBody*>(Manager->getComponent(gg::RIGID_BODY, getEntityID()));
    cDynamicModel = static_cast<CDynamicModel*>(Manager->getComponent(gg::DYNAMICMODEL, getEntityID()));
    // //std::cout << "llega" << '\n';
    hab = static_cast<CHabilityController*>(Manager->getComponent(gg::HAB, getEntityID()));
    //hab = static_cast<CHabilityController*>(Manager->getComponent(gg::HABILITY, getEntityID()));


    return gg::ST_TRUE;
}


void CPlayerController::Update(){
    int wheelState = Engine->getWheelState();
    if(wheelState!=0){
        actualGrenadeState -= wheelState;
        if(actualGrenadeState<1)    actualGrenadeState = 3;
        if(actualGrenadeState>3)    actualGrenadeState = 1;
        //gg::cout(" -- ACTUAL GRENADE SET: "+std::to_string(actualGrenadeState));
    }

    //if(clocker.ElapsedTime().Seconds() < 0.1){
        //Engine->Draw3DLine(cTransform->getPosition() + glm::vec3(0, 0.5, 0), Target, gg::Color(255, 0, 0));
    //}

}

void CPlayerController::FixedUpdate(){

    if(!cTransform || !camera || !ghostCollider)  return;

    if(collider->getBodyPosition().y < -65){
        reset();
        return;
    }
    // auto posJ=cTransform->getPosition();
    //
    // std::cout << posJ.x <<","<< posJ.y << "," << posJ.z << '\n';
    //332.327,-42.8137,60.2511


    // Vector que tendrá el impulso para aplicar al body
    force.x = 0;
    force.y = 0;
    force.z = 0;

    pressed = false;
    check_WASD(force, pressed);
    force *= 3.7*MULT_FACTOR;




    if(!s_pasos->isPlaying() && pressed){
      s_pasos->play();
    }


    for(uint8_t i = 0; i < KEYMAP.size(); ++i){
        if(Engine->key(KEYMAP[i].KEY, true))  (this->*KEYMAP[i].Target)();
    }

    if(!PlayerMovement)     return;


    if(pressed){
        if(cDynamicModel->getCurrentAnimation() != A_HERO::WALKING_WEAPON){
            cDynamicModel->ToggleAnimation(A_HERO::WALKING_WEAPON, 0.5);
        }
    }
    else{
        if(cDynamicModel->getCurrentAnimation() != A_HERO::STANDING){
            cDynamicModel->ToggleAnimation(A_HERO::STANDING, 2);
        }
    }



    bool AIM = Engine->isRClickPressed();

    aim(AIM);

    if(AIM){
        glm::vec3 Direction = ghostCollider->getVirtualRotation() * glm::vec3(0,0,1);
        camera->getDirectionVector(cV);
        cV.y = 0;
        if((cV.x || cV.z)) ghostCollider->setVirtualRotation(RotationBetween(Direction, cV));
    }
    else{
        glm::vec3 Direction = ghostCollider->getVirtualRotation() * glm::vec3(0,0,1);
        glm::vec3 Velocity = ghostCollider->getVelocity() * glm::vec3(-1, 0,-1);
        if((Velocity.x || Velocity.z) && glm::length2(Velocity) > 1) ghostCollider->setVirtualRotation(RotationBetween(Direction, Velocity));
    }

    collider->activate(true);
    if(pressed) collider->setLinearVelocity(glm::vec3(force.x, collider->getVelocity().y, force.z));
    else        collider->setLinearVelocity(glm::vec3(0      , collider->getVelocity().y, 0      ));

    glm::vec3 tmp = collider->getBodyPosition();
    ghostCollider->setBodyPosition(tmp);            // Para captar la fuerza del salto

    autoStepping();

    // DISPARO
    if(Engine->isLClickPressed()){
        // glm::vec3 STOESUNUPDATE_PERODEVUELVEUNAPOSICION = world->handleRayCast(camera->getCameraPosition(),cTransform->getPosition());
        Target = world->handleRayCast(camera->getCameraPosition(),camera->getTargetPosition());
        CGun* gun = static_cast<CGun*>(Manager->getComponent(gg::GUN, getEntityID()));
        clocker.Restart();
        if(gun) gun->shoot(Target);
    }


    if(secondWeapon) secondWeapon->fullDeBalas(1);

    // <DEBUG>
    //showDebug();
    // </DEBUG>
}

void CPlayerController::aim(const uint8_t &s){
    if(s) force *= 0.4;
    hud->aim(s);
}

// Código de los tutoriales de OpengGL, a optimisar
glm::quat CPlayerController::RotationBetween(glm::vec3 &V1, glm::vec3 &V2){
	V1 = normalize(V1);
	V2 = normalize(V2);

	float cosTheta = dot(V1, V2);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){

		rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), V1);
		if (glm::length2(rotationAxis) < 0.01 )
			rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), V1);

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(V1, V2);

	float s = sqrt( (1+cosTheta)*2 );
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);

}

void CPlayerController::playerThrowHolyBomb(){

    glm::vec3 gPos   = cTransform->getPosition();
    glm::vec3 from   = gPos;
    glm::vec3 to     = world->getRaycastVector();
    glm::vec3 vel    = to-from;

    vel  = glm::normalize(vel);
    vel *= GRENADE_VEL_FACTOR/2;

    factory->createHolyBomb(glm::vec3(gPos.x,gPos.y+GRENADE_THROW_Y_OFFSET,gPos.z),vel);
}

void CPlayerController::playerThrowMatrioska(){
    glm::vec3 gPos   = cTransform->getPosition();
    glm::vec3 from   = gPos;
    glm::vec3 to     = world->getRaycastVector();
    glm::vec3 vel    = to-from;

    vel  = glm::normalize(vel);
    vel *= GRENADE_VEL_FACTOR/2;

    factory->createMatriuska(glm::vec3(gPos.x,gPos.y+GRENADE_THROW_Y_OFFSET,gPos.z),vel);
}

void CPlayerController::playerThrowDopple(){
    glm::vec3 gPos   = cTransform->getPosition();
    glm::vec3 from   = gPos;
    glm::vec3 to     = world->getRaycastVector();
    glm::vec3 vel    = to-from;

    vel  = glm::normalize(vel);
    vel *= GRENADE_VEL_FACTOR/2;

    factory->createSenyuelo(glm::vec3(gPos.x,gPos.y+GRENADE_THROW_Y_OFFSET,gPos.z),vel);
}

void CPlayerController::check_WASD(glm::vec3 &force, bool &flag_pressed){
    int8_t W_S = Engine->key(gg::S) - Engine->key(gg::W);
    int8_t A_D = Engine->key(gg::A) - Engine->key(gg::D);

    if(W_S || A_D){
        flag_pressed = true;

        camera->getDirectionVector(cV);

        if(W_S) force = glm::vec3(cV.x * W_S, 0, cV.z * W_S);

        // Vector perpendicular al vector direccion
        if(A_D) force += glm::vec3(-cV.z*A_D, 0, cV.x*A_D);

        if(force.x || force.z)
            force = glm::normalize(force);
    }
}

#define RC_OFFSET           1.6f                // Max offset del auto-stepping
void CPlayerController::autoStepping(){
    // Auto-stepping
    glm::vec3 start = ghostCollider->getBodyPosition();
    glm::vec3 end = glm::vec3(start.x,start.y-(RC_OFFSET),start.z);
    glm::vec3 result;

    bool hit = world->RayCastTest(start,end,result,ghostCollider,collider);
    // bool hit = world->CompleteRayCastTest(start,end,result,ghostCollider,collider);

    if(hit){
        result.y += RC_OFFSET/1.7f;
        ghostCollider->setBodyPosition(result);

        start = collider->getLinearVelocity();

        collider->deactivateGravity();
        collider->setLinearVelocity(glm::vec3(start.x,0,start.z));

        if(isColliderGravitySet && GH_PREV != result){
          s_saltar->play();
        }

        if(isColliderGravitySet || GH_PREV != result){
            result.y += 0.5;
            collider->setNotKinematicBodyPosition(result);
            isColliderGravitySet = false;
            GH_PREV = result;
        }
    }
    else{
        collider->activateGravity();
        isColliderGravitySet = true;
        GH_PREV = glm::vec3(0);
    }

}

void CPlayerController::showDebug(){
    if(Engine->key(gg::F1)){
        if(!debug1){
            debug1 = true;
            debug2? debug2=false : debug2=true;
            world->setDebug(debug2);
        }
    }
    else{
        debug1 = false;
    }

    if(debug2){
        // DEBUG ACTIVATED
        // gg::cout(
        //     "(X:"+std::to_string(cTransform->getPosition().x)+
        //     ",Y:"+std::to_string(cTransform->getPosition().y)+
        //     ",Z:"+std::to_string(cTransform->getPosition().z)+")"
        // );
    }
}

bool CPlayerController::heroHasSecondWeapon(){
    if(secondWeapon)    return true;
    else                return false;
}

int CPlayerController::setSecondWeapon(CGun *_weapon){
    int ret;
    if(secondWeapon){
        ret = secondWeapon->getType();
    }
    else{
        ret = -1;
    }

    secondWeapon = _weapon;
    return ret;
}

void CPlayerController::SprintBuf(){
    MULT_BASE=1.5;
}

void CPlayerController::SprintDebuf(){
    MULT_BASE=1;
}

bool CPlayerController::canPickWeapon(){
    if(Engine->key(gg::E)){
        if(!pulsacion_f){
            pulsacion_f = true;
            if(!heroHasSecondWeapon()){
                auto estado = new PopState();
                AssetManager* Manager = Singleton<AssetManager>::Instance();
                estado->Addim(Manager->getTexture("assets/HUD/armas_esp.png"));
                estado->Addim(Manager->getTexture("assets/HUD/municion_esp.png"));
                Singleton<StateMachine>::Instance()->AddState(estado);
            }
            return true;
        }
    }
    else{
        pulsacion_f = false;
    }
    return false;
}

bool CPlayerController::hasItem(const uint16_t &_item){
    for(const uint16_t& i : items){
        if(i==_item){
            return true;
        }
    }
    return false;
}

bool CPlayerController::pickItem(const uint16_t &_item){
    // No compruebo duplicados entre items
    // Nunca pasara esa situacion
    for(const uint16_t &i : items){
        if(i==0){
            items[i] = _item;
            return true;
        }
    }
    return false;
}

bool CPlayerController::useItem(const uint16_t &_item){
    for(const uint16_t &i : items){
        if(i==_item){
            items[i] = 0;
            return true;
        }
    }
    return false;
}


void CPlayerController::ToggleSkill1(){
    hab->ToggleSkill(0);
}
void CPlayerController::ToggleSkill2(){
    hab->ToggleSkill(1);
    MULT_FACTOR = 1.7;
}
void CPlayerController::resetMultFactor(){
    MULT_FACTOR = 1;
}
void CPlayerController::ToggleSkill3(){
    if(cDynamicModel->getCurrentAnimation() != A_HERO::WAVE) {
        cDynamicModel->ToggleAnimation(A_HERO::WAVE, 0.15, true);
    }
    hab->ToggleSkill(2);
}

void CPlayerController::ReloadGun(){
    CGun* gun = static_cast<CGun*>(Manager->getComponent(gg::GUN, getEntityID()));
    if(gun && gun->canReload() && !gun->isReloading()){
        Manager->returnIDFromRigid(nullptr);
        gun->reload();
    }
}

void CPlayerController::ThrowGranade(){
    // glm::vec3 STOESUNUPDATE_PERODEVUELVEUNAPOSICION = world->handleRayCast(camera->getCameraPosition(),cTransform->getPosition());
    glm::vec3 STOESUNUPDATE_PERODEVUELVEUNAPOSICION = world->handleRayCast(camera->getCameraPosition(),camera->getTargetPosition());
    ////std::cout << actualGrenadeState << '\n';
    (this->*mapFuncGrenades[actualGrenadeState])();
}

void CPlayerController::ChangeWeapon(){
    if(secondWeapon){
        CGun *gun = static_cast<CGun*>(Manager->getComponent(gg::GUN,getEntityID()));
        if(!gun->isReloading()){
            // Singleton<Motor2D>::Instance()->setbullet(1,gun->getBullets(),gun->getTotalBullets());
            // Singleton<Motor2D>::Instance()->changeWeapon();

            // std::cout << "PRIMARY:  " << Manager->getComponent(gg::GUN,getEntityID()) << '\n';
            // std::cout << "SECONDARY:" << secondWeapon << '\n';
            Manager->swapComponents(gg::GUN,getEntityID(),reinterpret_cast<IComponent**>(&secondWeapon));
            secondWeapon->desenfundado();

            hud->changeHUDWeapon();
            // hud.setSecondaryChamber(123);

            // std::cout << "PRIMARY:  " << Manager->getComponent(gg::GUN,getEntityID()) << '\n';
            // std::cout << "SECONDARY:" << secondWeapon << '\n';
        }
    }
}

void CPlayerController::Run(){
    MULT_FACTOR = MULT_RUN_FACTOR;
}

void CPlayerController::DASH(){
    MULT_FACTOR = MULT_DASH_FACTOR;
    force.x *= DASH_FORCE_FACTOR;
    force.z *= DASH_FORCE_FACTOR;

    s_dash->play();
}

void CPlayerController::JUMP(){
    if(!canJump()) return;

    collider->applyCentralForce(glm::vec3(0, JUMP_FORCE_FACTOR, 0));
    if(pressed){
        if(cDynamicModel->getCurrentAnimation() != A_HERO::JUMPING_WALKING){
            s_saltarVoz->play();
            cDynamicModel->ToggleAnimation(A_HERO::JUMPING_WALKING, 0.2, true);
        }
    }
    else{
        if(cDynamicModel->getCurrentAnimation() != A_HERO::JUMPING){
            s_saltarVoz->play();
            cDynamicModel->ToggleAnimation(A_HERO::JUMPING, 0.2, true);
        }
    }
}

bool CPlayerController::canJump(){
    glm::vec3 start = cTransform->getPosition();
    glm::vec3 end = start;
    end.y -= 1.f;
    glm::vec3 result;

    return world->RayCastTest(start,end,result,ghostCollider,collider);
}

void CPlayerController::TogglePause() {
    Singleton<StateMachine>::Instance()->AddState(new PauseState(),false);
}

void CPlayerController::MostrarTexto(){
    //std::string texto[]{
    //    "Effects",
    //    "Effects",
    //    "Effects"
    //};
    //int nlineas=4;
    //Singleton<Motor2D>::Instance()->pintarTexto(nlineas,texto);
    auto estado = new PopState();
    AssetManager* _AssetManager = Singleton<AssetManager>::Instance();
    estado->Addim(_AssetManager->getTextureWithoutSavingToMap("assets/HUD/asdw_esp.png"));
    estado->Addim(_AssetManager->getTextureWithoutSavingToMap("assets/HUD/camara_esp.png"));
    estado->Addim(_AssetManager->getTextureWithoutSavingToMap("assets/HUD/dash_esp.png"));
    Singleton<StateMachine>::Instance()->AddState(estado, false);

    //Singleton<Motor2D>::Instance()->pintarImagen("assets/HUD/ultrasonido_esp.png");
}
// void CPlayerController::QuitarTexto(){
//     Singleton<Motor2D>::Instance()->InitHUD();
// }

void CPlayerController::ToggleFreeCamera(){
    camera->ToogleFreeCamera();

    if(!FreeCamera){
        PlayerMovement = false;
        FreeCamera = true;
    }
    else{
        PlayerMovement = true;
        FreeCamera = false;
    }
}
void CPlayerController::explosion(glm::vec3 vPos,float fuerzabomba){
    // float distancia=glm::distance(cTransform->getPosition(),vPos);
    glm::vec3 sol =glm::normalize(cTransform->getPosition()-vPos)*fuerzabomba;
    collider->applyCentralForce(sol);

}
void CPlayerController::reset(){
    //resetear la posicion del jugador
    //colider en pos //pos_jugador
    /*traza
    pos_jugador fixed (310.148,-43.975,130.425)
    pos_jugador fixed (310.148,-43.975,130.425)
    pos_jugador en reset (317.127,-24.6235,71.4132)
    pos_jugador fixed (310.156,-43.979,130.587)
    pos_jugador fixed (316.671,-25.8877,75.2786)
    pos_jugador fixed (310.632,-42.6559,126.577)
    */
    //std::cout << "pos_jugador en reset (" <<pos_jugador.x<<","<<pos_jugador.y<<","<<pos_jugador.z<<")"<< '\n';


    glm::vec3 ZONES_RESPAWNS[7] = {
        glm::vec3(33.225227, -21.338203, 35.246361),
        glm::vec3(129.612900, -20.939051, 62.093109),
        glm::vec3(217.182678, -24.362558, 39.726517),
        glm::vec3(329.710754, -43.133827, 61.127842),
        glm::vec3(295.498993, -43.432789, 156.500885),
        glm::vec3(239.640900, -43.432274, 245.919968),
        glm::vec3(35.163818, -44.162739, 258.325195)
    };

    uint8_t CURRENT_ZONE = 0;
    for(uint8_t i = 0; i < 7; ++i) {
        if(Engine->ZONES[i]->getVisibility())
            CURRENT_ZONE = i;
    }


    glm::vec3 pos_jugador = ZONES_RESPAWNS[CURRENT_ZONE];

    collider->activate(true);
    collider->setNotKinematicBodyPosition(pos_jugador);
    collider->setBodyPosition(pos_jugador);
    //collider->setXZPosition(pos_jugador.x, pos_jugador.z);

    int nCAgentID=Manager->getHeroID();

    //reset habilidades

    Manager->removeComponentFromEntity(gg::HAB,nCAgentID);
    CHabilityController* Hab                         = new CHabilityController();
    Manager->addComponentToEntity(Hab,             gg::HAB, nCAgentID);

    hab = static_cast<CHabilityController*>(Manager->getComponent(gg::HAB, nCAgentID));

    //eliminar anyadir componente

    //reset arma
    CGun *gun = static_cast<CGun*>(Manager->getComponent(gg::GUN,nCAgentID));
    if(gun){
        //std::cout << "reseteamos arma" << '\n';
        gun->reset();
    }
    if(secondWeapon){
        secondWeapon->reset();
    }

    Singleton<AIDirector>::Instance()->restart();

    //ponemos pop up muertes
    PopState* aux = new PopState();
    AssetManager* Manager = Singleton<AssetManager>::Instance();
    aux->Addim(Manager->getTextureWithoutSavingToMap("assets/HUD/muerte_esp.png"));
    //estado->Addim(Manager->getTextureWithoutSavingToMap("assets/HUD/camara_esp.png"));
    Singleton<StateMachine>::Instance()->AddState(aux);

    /*
    int _wtype_floor=gun->getType();
    float   dmg, cdc, relDT, rng;
    std::string sonido_disparo, sonido_recarga, sonido_desenfundado, sonido_vacia,imgP,imgS;
    int     cb,tb;

    gg::getWeaponInformation(dmg,cdc,relDT,rng,cb,tb,_wtype_floor, sonido_disparo,sonido_recarga,sonido_desenfundado,sonido_vacia,imgP,imgS);
    // Elimino el arma que tiene la entidad
    Manager->removeComponentFromEntity(gg::GUN,nCAgentID);
    // Le anyado la nueva
    CGun* Gun = new CGun(dmg,cdc,cb,tb,relDT,rng,_wtype_floor,sonido_disparo,sonido_recarga,sonido_desenfundado,sonido_vacia);
    Manager->addComponentToEntity(Gun, gg::GUN, nCAgentID);
    */
    //eliminar anyadir componente

}
void CPlayerController::EnemyInfo(){
    //devuelve ide de un objeto
    glm::vec3 STOESUNUPDATE_PERODEVUELVEUNAPOSICION = world->handleRayCast(camera->getCameraPosition(),camera->getTargetPosition(),200);
    int id=world->getIDFromRaycast();
    ////std::cout << "id:" <<id<< '\n';
    if(id!=-1){

        CAIEnem* AIEnem = static_cast<CAIEnem*>(Manager->getComponent(gg::AIENEM,id));
        if(AIEnem){
            ////std::cout << "no hay enemigo" << '\n';
            Singleton<StateMachine>::Instance()->AddState(new IAState(id),false);


        }
    }
    //gun->shoot(STOESUNUPDATE_PERODEVUELVEUNAPOSICION);
}
