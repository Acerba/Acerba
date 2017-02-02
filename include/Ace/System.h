#pragma once

#include <Ace/ComponentHandle.h>
#include <Ace/Component.h>

#include <algorithm>
#include <memory>
#include <vector>

//TODO: will we make a custom assert?
#include <cassert>

namespace ace
{



    class SystemBase
    {



        static UInt32 m_enabledComponentsCount;

        const UInt32 m_ID;

    protected:


        SystemBase(const UInt32 id) :
            m_ID(id)

        {

        }

        virtual ~SystemBase();

    public:

        virtual void Update() = 0;

    };




    template <typename CompType>
    class System : protected SystemBase
    {
        std::vector<Component<CompType>> m_components;
        std::vector<ComponentHandle<CompType>> m_handles;


        // SystemID == ComponentID
        System() :
            SystemBase(typeid(*this))
        {

        }

        virtual ~System();

        template <typename T, typename ... Args>
        ComponentHandle<T>& _addComponentToEntity(const UInt32 entityID, Args&& ... args)
        {
            m_components.emplace_back(entityID, std::forward<Args>(args)...);
            m_handles.emplace_back(&m_components.back());
            return m_handles.back();
        }

        
    public:

        /// \brief Get a system reference of type CompType
        static System& GetInstance()
        {
            static System instance;
            return instance;
        }
        //////////////////////////////


        //   /// \brief Adds a component of type CompType and returns a handle to it
        //   /// \param args Variadic arguments needed to construct the desired component
        //   template <typename ... Args>
        //   ComponentHandle<CompType>& AddComponent(Args&& ... args)
        //   {
        //       m_components.emplace_back(std::forward<Args>(args)...); //Create component
        //       m_handles.emplace_back(m_components.back()->GetID());   //Create handle with component ID
        //       return m_handles.back();                                //Return reference to handle
        //   }
        //   //////////////////////////////


        /// \brief Disable an existing component from use (Does not remove it)
        /// \param handle Handle of the component to disable
        /// \return Boolean True if the component was disabled
        bool DisableComponent(ComponentHandle<CompType>* handle)
        {
            bool decrease = false;
            for (size_t i = 0u; i < m_enabledComponentsCount; ++i)
            {
                if (&m_components[i] == handle->operator()())
                {
                    //TODO: is this necessary
                    if (i == m_enabledComponentsCount - 1u)
                    {
                        decrease = true;
                        break;
                    }

                    auto itr = m_components.begin() + i;
                    std::rotate(it, it + 1, m_components.end());
                    decrease = true;
                }
            }

            
            if (decrease)
                m_enabledComponentsCount == 0u ? assert(false) : --m_enabledComponentsCount;

            //  const size_t size = m_handles.size();
            //
            //  //Remove-erase handle whose ID == ID of the param handle
            //  m_handles.erase(
            //      std::remove_if(
            //      m_handles.begin(), m_handles.end(),
            //      [&handle](ComponentHandle<CompType>& ch){return ch.GetHandleID() == handle.GetHandleID(); }
            //  ), m_handles.end());
            //
            //  return size > m_handles.size();
        }
        //////////////////////////////


        /// \brief Enable an existing component for use (Will not be created if does not exist)
        /// \param componentID ID of the component to enable
        /// \return Boolean True if the component was enabled
        bool EnableComponent(const UInt32 componentID)
        {
            for (const auto& itr : m_components)
            {
                if (itr->GetID() == componentID)
                {
                    m_handles.emplace_back(ComponentHandle<CompType>(componentID));
                    return true;
                }
            }
            return false;
        }
        //////////////////////////////


        /// \brief Completely removes a component from use
        /// \param handle Handle of the component to remove
        /// \return Boolean True if successfully removed
        bool RemoveComponent(ComponentHandle<CompType>& handle)
        {
            const size_t sizeC = m_components.size();
            const size_t sizeH = m_handles.size();

            //Remove-erase component whose ID == ID pointed to by handle
            m_components.erase(
                std::remove_if(
                m_components.begin(), m_components.end(),
                [&handle](Component<CompType>& c){return c.GetID() == handle.GetCompID(); }
            ), m_components.end());

            //Remove-erase handle whose ID == ID of the param handle
            m_handles.erase(
                std::remove_if(
                m_handles.begin(), m_handles.end(),
                [&handle](ComponentHandle<CompType>& ch){return ch.GetHandleID() == handle.GetHandleID(); }
            ), m_handles.end());

            return
                sizeC > m_components.size() &&
                sizeH > m_handles.size();
        }
        //////////////////////////////





    };


}