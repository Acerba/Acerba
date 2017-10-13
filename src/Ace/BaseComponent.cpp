#include <Ace/BaseComponent.h>
#include <Ace/Entity.h>

namespace ace
{
    BaseComponent::BaseComponent(Entity& entity) :
        entity(entity), transform(&entity->transform)
    {

    }

    void BaseComponent::operator=(const BaseComponent& base)
    {

    }
}