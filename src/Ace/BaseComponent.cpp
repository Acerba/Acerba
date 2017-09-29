#include <Ace/BaseComponent.h>
#include <Ace/Entity.h>

namespace ace
{
    BaseComponent::BaseComponent(const Entity& entity) :
        entity(&entity), transform(&entity->transform)
    {

    }
}