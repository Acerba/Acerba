#pragma once

#include <Ace/IntTypes.h>


namespace
{
    static ace::UInt32 componentIDCounter = 0u;
    static ace::UInt32 componentIDCounterRemoved = 0u;
}

namespace ace
{

    template <typename T>
    class Component
    {

        UInt32 m_ID;


    public:

        UInt32 GetID() const
        {
            return m_ID;
        }


        Component() :
            m_ID(++componentIDCounter)
        {

        }

        ~Component()
        {
            ++componentIDCounterRemoved;
        }

    };


}