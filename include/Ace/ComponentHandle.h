#pragma once

#include <Ace/IntTypes.h>

#include <memory>

namespace
{
    static ace::UInt32 componentHandleIDCounter = 0u;
    static ace::UInt32 componentHandleIDCounterRemoved = 0u;
}

namespace ace
{

    template <typename T>
    class ComponentHandle
    {
        UInt32 m_compID;
        UInt32 m_handleID;

        //Should have * to component...

        ComponentHandle() :
            m_handleID(++componentHandleIDCounter),
            m_compID(0u)
        {

        }

        ComponentHandle(UInt32 componentID) :
            m_handleID(++componentHandleIDCounter),
            m_compID(componentID)
        {

        }

        ~ComponentHandle()
        {
            ++componentHandleIDCounterRemoved;
        }


    public:

        //Component* GetComp()
        //{
        //    return m_component.get();
        //}

        UInt32 GetCompID() const
        {
            return m_compID;
        }

        UInt32 GetHandleID() const
        {
            return m_handleID;
        }


    };
}