#ifndef _CAIENEM_H
#define _CAIENEM_H

#include <cstdint>
#include <list>
#include <vector>
#include <Util.hpp>
#include <Singleton.hpp>

#include <EventSystem/EnumTriggerType.hpp>
#include <EventSystem/CTriggerSystem.hpp>
#include "EventSystem/Blackboard.hpp"

#include <ComponentArch/IComponent.hpp>         // [OBLIGATORIO]
#include <ComponentArch/Message.hpp>
#include <ComponentArch/Enum.hpp>
#include <Bullet/ggDynWorld.hpp>

#include <ComponentArch/Components/CClock.hpp>
#include <ComponentArch/Components/CDynamicModel.hpp>

#include <Omicron/Omicron.hpp>            // [OPCIONAL] Si necesitas acceder a algún método de GameEngine
#include <GameAI/Treecontroller.hpp>

#include <FMOD/SoundSystem.hpp>
#include <FMOD/SonidoNormal.hpp>

#include <AnimationLUT.hpp>


class Treecontroller;

class CAgent;
class CTransform;
class CNavmeshAgent;
class ggDynWorld;

class ObjectManager;

class CAIEnem : public IComponent {
    public:
        CAIEnem(gg::EEnemyType _type, float _agresividad, glm::vec3 _playerPos, bool _playerSeen);
        CAIEnem(const CAIEnem &orig) = delete;
        virtual ~CAIEnem();

        // Functions of IComponent
        virtual gg::EMessageStatus processMessage(const Message &m);    // [OPCIONAL] (Obligatorio si referencias a otras componentes)
        virtual void Init();
        virtual void FixedUpdate();
        virtual void Update();

        // Handlers                                 // Funciones que se llaman dependiendo del mensaje que recibas
        gg::EMessageStatus MHandler_SETPTRS ();     // IMPORTANTE: SETPTRS Se usará para inicializar punteros a otras componentes
        void MHandler_SENYUELO(TriggerRecordStruct* cdata);
        void MHandler_SENYUELO_END();
        void MHandler_NEAR(TriggerRecordStruct* cdata){}
        void MHandler_ATURD();

        void resetMyOwnTree();

        void enemyseen();
        void enemyrange();

        void setPlayerIsAttacking(bool);
        bool getPlayerIsAttacking();

        void setCloserAllyIsDead(bool);
        bool getCloserAllyIsDead();
        float getArange();

        void setImAttacking(bool);
        bool getImAttacking();

        void setSigno(int);
        int getSigno();

        bool getPlayerSeeing();

        void upgradeMaxAliensAttackingAtOnce();
        int getMaxAliensAttackingAtOnce();

        void upgradeRage();
        float getRage();

        void resetHabilityUpdateCounter();
        int getHabilityUpdateCounter();

        void explosiveWave();

        int getEnemyType();
        float getVelocity();
        void checkzona();

        void playMovement();
        void playAttack();
        void playAttack2();
        // /////////////////////////////////////////////// //
        // JAVI CAMBIA LO DE LAS VARIABLES PUBLICAS !      //
        // /////////////////////////////////////////////// //

        static CTransform* PlayerTransform;     //  Punteros a otras componentes
        static CRigidBody* PlayerBody;     //  Punteros a otras componentes
        Treecontroller* arbol;
        int zona;
        glm::vec3 playerPos;
        glm::vec3 senpos;
        glm::vec3 destino;
        float enfado;
        float agresividad;
        float Vrange;
        float Arange;
        float gradovision;

        Blackboard* data;

        int signo;
        int ID;
        int ultrasonido_cont;
        int rondacion_cont;


        bool playerSeen;
        bool playerOnRange;
        bool playerSeeing;
        bool ultrasonido;
        bool senyuelo;
        bool            CanIReset;

        inline CRigidBody* getCollider(){ return collider; };
        void moveBodies(const glm::vec3 &vel);

    private:
        Omicron      *Engine;
        ObjectManager   *Manager;
        CTriggerSystem  *EventSystem;
        CTransform      *cTransform;

        // ----------------------------
        CRigidBody      *ghostCollider;
        CRigidBody      *collider;
        glm::vec3 GH_PREV;
        bool isColliderGravitySet;
        // ----------------------------

        CAgent          *cAgent;
        ggDynWorld      *world;
        CDynamicModel   *cDynamicModel;
        // CNavmeshAgent   *nvAgent;

        gg::EEnemyType type;
        int             numberOfUpdatesSinceLastHability;
        int             maxAliensAttacking;
        float             velocity;

        static bool     debugvis;
        bool            imAttacking;
        bool            isPlayerAttacking;
        bool            closerAllyIsDead;

        SoundSystem* SS;

        SoundEvent* s_caminar;
        SoundEvent* s_atacar;
        SoundEvent* s_grito;
        //bool            CanIReset;

        void enableVisualDebug();
};

#endif
