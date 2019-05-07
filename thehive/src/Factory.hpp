#ifndef FACTORY_H
#define FACTORY_H

#include <ComponentArch/ObjectManager.hpp>
#include <Omicron/Omicron.hpp>
#include <EventSystem/CTriggerSystem.hpp>
#include <Util.hpp>

#include <EventSystem/Blackboard.hpp>
#include <EventSystem/BRbData.hpp>
#include <EventSystem/BFloat.hpp>

#include <FMOD/SoundSystem.hpp>
#include <FMOD/SonidoNormal.hpp>

template <typename T>
class Singleton;

class Factory{
    friend class Singleton<Factory>;

    public:
        uint16_t createHero(const glm::vec3 &Position,int8_t);
        uint16_t createSoldier(StandardNode* FATHER, const glm::vec3&,const float&);
        uint16_t createSoldierHorda(const glm::vec3&,const float&,const glm::vec3&);
        uint16_t createSoldierWandering(const glm::vec3&,const float&);
        uint16_t createRusher(const glm::vec3&,const float&);
        uint16_t createTank(const glm::vec3&,const float&);
        uint16_t createSingleSwarm(StandardNode* FATHER, const glm::vec3&,const float&);
        //uint16_t createSingleSwarm(const glm::vec3 &Position,const float &health)

        //CRigidBody* createSingleSwarm( const glm::vec3 &Position);
        uint16_t createSwarm(const glm::vec3 &Position,const float &health);

        uint16_t createNatureMesh(StandardNode* FATHER, const std::string &Path, const glm::vec3 &Position, const glm::quat &Rotation, ZMaterial *Material);


        // uint16_t createCollisionableStaticModel(const std::string &Path, const std::string &BulletPath, const std::string &Texture, const glm::vec3 &Position);
        // uint16_t createCollisionableDynamicModel(const std::string &Path, const std::string &BulletPath, const std::string &Texture, const glm::vec3 &Position);
        // uint16_t createStaticModel(const std::string &Path, const std::string &Texture, const glm::vec3 &Position, const glm::vec3 &Rotation = glm::vec3());
        //uint16_t createPathFindingActor(const glm::vec3 &Position);
        uint16_t createHolyBomb(const glm::vec3 &Position, const glm::vec3 &Impulse);
        uint16_t createMatriuska(const glm::vec3 &Position, const glm::vec3 &Impulse);
        uint16_t createSenyuelo(const glm::vec3 &Position, const glm::vec3 &Impulse);
        uint16_t createCollectableWeapon(const glm::vec3&, int);
        uint16_t createPickableItem(StandardNode* FATHER, const glm::vec3&);
        uint16_t createTouchableObject(StandardNode* FATHER, const std::string&, const glm::vec3&, const glm::quat &_rotation, const uint16_t&, const glm::vec3&, const int&, int, uint16_t=0);
        //uint16_t createDebugBullet(StandardNode* FATHER, const glm::vec3&);
    private:

        Factory();
        Factory(const Factory &orig) = delete;
        void operator=(const Factory &orig) = delete;

        ObjectManager* Manager;
        Omicron* Engine;

        //SoundSystem* SS;

        //SoundEvent* s_gotera;

};

#endif
