#pragma once

#include <Ace/Macros.h>
#include <Ace/TypesFwd.h>

namespace ace
{
    class Entity;
    

    class Scene final
    {

        Entity* m_root;

        ACE_DISABLE_COPY(Scene)

    public:

        /**
            @brief Scene constructor.
            @param[in, out] rootManager EntityManager to handle this scene. Uses DefaultManger by default.
        */
        Scene(class EntityManager* rootManager = nullptr);

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

        /**
            @brief Draw the world and all its children from the perspective of the camera
            @param[in] camera Camera from which to draw
            @param[in] material Override all children materials
         */
        void Draw(const class Camera& camera, const struct Material* material = nullptr) const;

        /**
            @brief Update relative positions
         */
        void Update();



    };
}