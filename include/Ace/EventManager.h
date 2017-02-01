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
        //Container of event pointers
        std::vector<EventBase<ParamT>*> m_events;

        EventManager() :
            m_events()
        {

        }

        ~EventManager()
        {

        }

    public:

        //Singleton
        static EventManager& GetInstance()
        {
            static EventManager manager;
            return manager;
        }

        //Add event to container
        static void Add(EventBase<ParamT>& evnt)
        {
            EventManager::GetInstance().m_events.emplace_back(&evnt);
        }

        //Removes event from container
        static void Remove(EventBase<ParamT>& evnt)
        {
            auto& m = EventManager::GetInstance();

            //Remove-erase
            m.m_events.erase(std::remove_if(
                m.m_events.begin(), m.m_events.end(),
                [&evnt](EventBase<ParamT>* e){return e == &evnt; }
            ), m.m_events.end());
        }

        //Broadcast to all events listening to ParamT
        static void Broadcast(ParamT paramt)
        {
            for (auto& itr : EventManager<ParamT>::GetInstance().m_events)
            {
                itr->OnEvent(paramt);
            }
        }

    };


    template <typename ParamT>
    class EventBase
    {
    public:

        //Event lives with the parenting object
        EventBase()
        {
            EventManager<ParamT>::Add(*this);
        }

        //Event dies along with parenting object
        ~EventBase()
        {
            EventManager<ParamT>::Remove(*this);
        }

        //Override this method for custom logic
        virtual void OnEvent(ParamT) = 0;
    };

}
