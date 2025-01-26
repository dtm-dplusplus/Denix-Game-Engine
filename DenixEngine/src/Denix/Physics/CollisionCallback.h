#pragma once
#include <PxSimulationEventCallback.h>

namespace Denix
{
    using namespace physx;
    
    class CollisionCallback : public PxSimulationEventCallback
    {
    public:
        void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;

        void onConstraintBreak(PxConstraintInfo*, PxU32) override {}
        void onWake(PxActor**, PxU32) override {}
        void onSleep(PxActor**, PxU32) override {}
        void onTrigger(PxTriggerPair* pairs, PxU32 count) override{}
        void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override{}
        
    };
}
