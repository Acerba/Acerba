#pragma once

#include <Ace/IntTypes.h>

namespace ace
{

    template <typename U>
    class UUID
    {
        const UInt32 m_ID;

        inline static UInt32 GenerateID()
        {
            static UInt32 s_id = 0u;
            return ++s_id;
        }

        template <typename T>
        class ID_Handle
        {
            UInt32 m_ID;

            ID_Handle(const UInt32 id) :
                m_ID(id)
            {
                
            }

            inline static ID_Handle& GetInstance()
            {
                static ID_Handle handle(GenerateID());
                return handle;
            }

        public:

            inline static UInt32 GetID()
            {
                return GetInstance().m_ID;
            }
        };

    public:

        template <typename T>
        inline static UInt32 GetID(const T&)
        {
            return ID_Handle<T>::GetID();
        }
        template <typename T>
        inline static UInt32 GetID(const T*)
        {
            return ID_Handle<T>::GetID();
        }
        template <typename T>
        inline static UInt32 GetID()
        {
            return ID_Handle<T>::GetID();
        }

        template <typename T>
        UUID(const T& value) :
            m_ID(GetID(value))
        {

        }

        UUID(const UInt32 id) :
            m_ID(id)
        {

        }

        UUID& operator=(const UUID&)
        {
            // BVH implementation requires this
            return *this;
        }

        inline operator UInt32() const
        {
            return m_ID;
        }
    };


}