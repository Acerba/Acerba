//#pragma once
//
//#include <Ace/IntTypes.h>
//
//#include <memory>
//
//namespace ace
//{
//
//    template <typename T>
//    class ComponentHandle : public Component
//    {
//
//        //Should have * to component...
//
//        ComponentHandle() 
//        {
//
//        }
//
//        ComponentHandle(UInt32 componentID) :
//            m_handleID(++componentHandleIDCounter),
//            m_compID(componentID)
//        {
//
//        }
//
//        ~ComponentHandle()
//        {
//            ++componentHandleIDCounterRemoved;
//        }
//
//
//    public:
//
//        //Component* GetComp()
//        //{
//        //    return m_component.get();
//        //}
//
//        UInt32 GetCompID() const
//        {
//            return m_compID;
//        }
//
//        UInt32 GetHandleID() const
//        {
//            return m_handleID;
//        }
//
//
//    };
//}