#pragma once

#include <Ace/Macros.h>

namespace ace
{
    class Entity;
    class EntityManager;
    namespace math
    {
        class Matrix4;
    }
    

    class Scene
    {

        Entity* m_root;


        void ComputeMatrices(Entity& entity, const math::Matrix4& parentModel);


        ACE_DISABLE_COPY(Scene)

    public:

        /**
            @brief Scene constructor.
            @param[in, out] rootManager EntityManager to handle this scene. Uses DefaultManger by default.
        */
        Scene(EntityManager* rootManager = nullptr);

        /**
            @brief Scene destructor.
        */
        ~Scene();

        /**
            @brief Returns constant scene root entity.
            @return Constant Entity pointer.
        */
        const Entity& GetRoot() const;

        /**
            @brief Returns scene root entity.
            @return Entity pointer.
        */
        Entity& GetRoot();


        void Update();


    };
}