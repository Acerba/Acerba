#pragma once

#include <Ace/IntTypes.h>

#include <Ace/System.h>

#include <vector>

namespace
{
    static ace::UInt32 entityIDCounter = 0u;
    static ace::UInt32 entityIDCounterRemoved = 0u;
}

namespace ace
{

    //Entity should have components of many types...
    template <typename T>
    class Entity
    {
        //Should use shared_ptr: system has the same handles
        //Shouldn't use shared_ptr: user has access to this
        std::vector<ComponentHandle<T>> m_handles;

        UInt32 m_ID;

    public:

        UInt32 GetID() const
        {
            return m_ID;
        }

        Entity() :
            m_components(),
            m_ID(++componentIDCounter)
        {

        }

        ~Entity()
        {
            ++componentIDCounterRemoved;
        }



    };

}