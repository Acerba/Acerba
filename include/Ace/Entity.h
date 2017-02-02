#pragma once

#include <Ace/IntTypes.h>

#include <Ace/System.h>

#include <vector>


namespace ace
{

    class EntityManager
    {
        EntityManager();
        virtual ~EntityManager();

    public:

        static EntityManager& GetInstance()
        {
            static EntityManager instance;
            return instance;
        }



    };

    class Entity
    {
        //Should use shared_ptr: system has the same handles
        //Shouldn't use shared_ptr: user has access to this
        //std::vector<ComponentHandle> m_handles;

        UInt32 m_ID;

    public:

        UInt32 GetID() const;

        Entity();
        ~Entity();

        template <typename T, typename ... Args>
        ComponentHandle<T>& AddComponent(Args&& ... args)
        {
            return System<T>::GetInstance()
                ._addComponentToEntity(m_ID, std::forward<Args>(args)...);
        }


    };

}