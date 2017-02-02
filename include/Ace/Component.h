#pragma once

#include <Ace/IntTypes.h>

#include <typeinfo>



namespace ace
{

    

    class ComponentBase
    {

        UInt32 m_entityID;

    public:

        UInt32 GetOwner() const
        {
            return m_entityID;
        }

        void SetOwner(const UInt32 entityID)
        {
            m_entityID = entityID;
        }

        ComponentBase(const UInt32 entityId) :
            m_entityID(entityId)
        {

        }

        virtual ~ComponentBase();

    };


    template <typename T>
    class Component : public ComponentBase
    {

    public:

        Component(const UInt32 entityID) :
            ComponentBase(entityID)
        {

        }

        virtual ~Component();

    };


    template <typename T>
    class ComponentHandle
    {

        Component<T>* m_component;

    public:

        Component<T>* operator()()
        {
            return m_component;
        }

        ComponentHandle(Component<T>* compPtr) :
            m_component(compPtr)
        {

        }

        ~ComponentHandle()
        {

        }

    };


}