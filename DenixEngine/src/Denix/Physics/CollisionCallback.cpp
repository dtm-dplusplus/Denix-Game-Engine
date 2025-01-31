#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "CollisionCallback.h"
#include "Denix/Scene/Actor.h"

void Denix::CollisionCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs,
                                         PxU32 nbPairs)
{
    for (PxU32 i = 0; i < nbPairs; i++)
    {
        const PxContactPair& contact = pairs[i];

        // Get Contact Data
        std::array physComps =
        {
            static_cast<PhysicsComponent*>(pairHeader.actors[0]->userData),
            static_cast<PhysicsComponent*>(pairHeader.actors[1]->userData)
        };

        CollisionPair collision;
        collision.m_Actors = {physComps[0], physComps[1]};

        if (contact.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            //collisionPair.Point = {contacts[0].position.x, contacts[0].position.y, contacts[0].position.z};
            //collisionPair.Normal = {contacts[0].normal.x, contacts[0].normal.y, contacts[0].normal.z};
            //collisionPair.Impulse = {contacts[0].impulse.x, contacts[0].impulse.y, contacts[0].impulse.z};

            collision.CollisionEnter = true;

            // Register Collision Data to Game Engine
            for (auto& comp : physComps)
                if (comp) comp->m_CollisionData.push_back(collision);
        }

        if (contact.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            collision.CollisionEnter = false;

            for (auto& comp : physComps)
                if (comp && !comp->IsRubbish()) comp->m_CollisionData.push_back(collision);
        }
    }
}
