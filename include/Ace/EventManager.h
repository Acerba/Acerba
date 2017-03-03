#pragma once

#include <algorithm>
#include <vector>


namespace ace
{
    template <typename ParamT>
    class EventBase;

    template <typename ParamT>
    class EventManager
    {

        std::vector<EventBase<ParamT>*> m_events;

        EventManager() :
            m_events()
        {

        }

        ~EventManager()
        {

        }

    public:

        /**
        @brief Singleton instance retriever.
        @return EventManager reference.
        */
        static EventManager& GetInstance()
        {
            static EventManager manager;
            return manager;
        }

        /**
        @brief Add event to container.
        @param[in, out] evnt Event to add to container.
        */
        static void Add(EventBase<ParamT>& evnt)
        {
            EventManager::GetInstance().m_events.emplace_back(&evnt);
        }

        /**
        @brief Removes event from container.
        @param[in, out] evnt Event to remove from container.
        */
        static void Remove(EventBase<ParamT>& evnt)
        {
            auto& m = EventManager::GetInstance();

            //Remove-erase
            m.m_events.erase(std::remove_if(
                m.m_events.begin(), m.m_events.end(),
                [&evnt](EventBase<ParamT>* e){return e == &evnt; }
            ), m.m_events.end());
        }


        /**
        @brief Broadcast to all events listening to ParamT.
        @param[in, out] param Parameter to broadcast.
        */
        static void Broadcast(ParamT param)
        {
            for (auto& itr : EventManager<ParamT>::GetInstance().m_events)
            {
                itr->OnEvent(param);
            }
        }

    };


    template <typename ParamT>
    class EventBase
    {
    public:

        /**
        @brief Event lives until its parenting object dies
        */
        EventBase()
        {
            EventManager<ParamT>::Add(*this);
        }

        /**
        @brief Event dies along with parenting object
        */
        ~EventBase()
        {
            EventManager<ParamT>::Remove(*this);
        }

        /**
        @brief Overridable function for custom logic.
        @param[in] param Parameter for event.
        */
        virtual void OnEvent(ParamT param) = 0;
    };

}
