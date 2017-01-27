#include <Ace/Module.h>

#include <Ace/Event.h>
#include <Ace/EventManager.h>

#include <iostream>

namespace ace
{

    class MyEvent : public EventBase<int>
    {
    public:
        void OnEvent(int param)
        {
            std::cout << param << " kjbdkjngdshfkjhhfdjhhgfdkjhhgd" << std::endl;
        }
    };
}

int main(int, char**)
{
    ace::Init();


    //ace::Event::Update();
    {
        ace::MyEvent eventti;

        eventti.OnEvent(400);

    }
    ace::EventManager<int>::Broadcast(300);


    return 0;
}