#ifndef _ICOMPONENT_H
#define _ICOMPONENT_H

#include "Enum.hpp"
#include "Message.hpp"
#include <cstdint>

class IComponent{
    //  We make this class friend of ObjectManger, so it can access the setEntityID method
    friend class ObjectManager;
    public:
        IComponent(const IComponent &orig) = delete;
        virtual ~IComponent();

        //  ---
        //  Returns the EntityID
        //========================================================================
        uint16_t getEntityID();

        //virtual gg::EComponentType getComponentType() = delete;

        //  ---
        //  Process the message and returns a message status
        //========================================================================
        virtual gg::EMessageStatus processMessage(const Message &m);

        //  ---
        //  Initializes the component data the moment the component is created
        //========================================================================
        virtual void Init();

        //  ---
        //  This method is called once every 1/60 seconds.
        //========================================================================
        virtual void Update();

        //  ---
        //  This method is called once every 1/15 seconds. Use it for calculations
        //========================================================================
        virtual void FixedUpdate();

    protected:
        IComponent();

    private:

        uint16_t EntityID;

        //  ---
        //  We just set the EntityID
        //========================================================================
        void setEntityID(uint16_t _ID);

};

#endif
