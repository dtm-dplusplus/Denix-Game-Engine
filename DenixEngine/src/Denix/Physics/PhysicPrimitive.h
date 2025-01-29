#pragma once

#include <cstdint>

enum class ColliderType
{
    Plane,
    Cube,
    Sphere
};

// Define bit flags for physics attributes
enum PhysicsAttributeFlags
{
    PHYSICS_SIMULATE = (1 << 0), // Simulate modified
    PHYSICS_COLLISION = (1 << 1), // Collision modified
    PHYSICS_MASS = (1 << 2), // Mass modified
    PHYSICS_LINEAR_DRAG = (1 << 3), // Linear Drag modified
    PHYSICS_ANGULAR_DRAG = (1 << 4), // Angular Drag modified
    PHYSICS_MATERIAL = (1 << 5), // Elasticity modified
    PHYSICS_SHAPE = (1 << 6), // Transform (Moveability, Scale, rotation) modified
};

// Enable bitwise operations for PhysicsAttributeFlags
inline PhysicsAttributeFlags operator|(PhysicsAttributeFlags lhs, PhysicsAttributeFlags rhs)
{
    return static_cast<PhysicsAttributeFlags>(
        static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs)
    );
}

inline PhysicsAttributeFlags& operator|=(PhysicsAttributeFlags& lhs, PhysicsAttributeFlags rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

inline PhysicsAttributeFlags operator&(PhysicsAttributeFlags lhs, PhysicsAttributeFlags rhs)
{
    return static_cast<PhysicsAttributeFlags>(
        static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs)
    );
}

inline PhysicsAttributeFlags& operator&=(PhysicsAttributeFlags& lhs, PhysicsAttributeFlags rhs)
{
    lhs = lhs & rhs;
    return lhs;
}

inline PhysicsAttributeFlags operator~(PhysicsAttributeFlags flag)
{
    return static_cast<PhysicsAttributeFlags>(~static_cast<uint32_t>(flag));
}

static physx::PxFilterFlags PhysicsFilterShader(
    physx::PxFilterObjectAttributes attributes0,
    physx::PxFilterData filterData0,
    physx::PxFilterObjectAttributes attributes1,
    physx::PxFilterData filterData1,
    physx::PxPairFlags& pairFlags,
    const void* constantBlock,
    physx::PxU32 constantBlockSize)
{
    pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;

    // Report when there is any contact between the two objects
    pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
    return physx::PxFilterFlag::eNOTIFY;
}
