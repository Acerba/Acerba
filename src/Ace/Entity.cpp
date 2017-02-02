#include <Ace/Entity.h>

namespace ace
{

    static UInt32 _entityIDCounter = 0u;

    Entity::Entity() :
        m_ID(++_entityIDCounter)
    {

    }

    Entity::~Entity()
    {

    }

    UInt32 Entity::GetID() const
    {
        return m_ID;
    }

}