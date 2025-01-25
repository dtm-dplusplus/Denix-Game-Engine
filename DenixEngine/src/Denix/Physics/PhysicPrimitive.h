#pragma once

#include <cstdint>

// Define bit flags for physics attributes
enum PhysicsAttributeFlags
{
    PHYSICS_SIMULATE     = (1 << 0),    // Simulate modified
    PHYSICS_COLLISION     = (1 << 1),     // Collision modified
    PHYSICS_MASS    = (1 << 2),     // Mass modified
    PHYSICS_LINEAR_DRAG    = (1 << 3),     // Linear Drag modified
    PHYSICS_ANGULAR_DRAG = (1 << 4),     // Angular Drag modified
    PHYSICS_MATERIAL = (1 << 5),     // Elasticity modified
};

// Enable bitwise operations for PhysicsAttributeFlags
inline PhysicsAttributeFlags operator|(PhysicsAttributeFlags lhs, PhysicsAttributeFlags rhs) {
    return static_cast<PhysicsAttributeFlags>(
        static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs)
    );
}

inline PhysicsAttributeFlags& operator|=(PhysicsAttributeFlags& lhs, PhysicsAttributeFlags rhs) {
    lhs = lhs | rhs;
    return lhs;
}

inline PhysicsAttributeFlags operator&(PhysicsAttributeFlags lhs, PhysicsAttributeFlags rhs) {
    return static_cast<PhysicsAttributeFlags>(
        static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs)
    );
}

inline PhysicsAttributeFlags& operator&=(PhysicsAttributeFlags& lhs, PhysicsAttributeFlags rhs) {
    lhs = lhs & rhs;
    return lhs;
}

inline PhysicsAttributeFlags operator~(PhysicsAttributeFlags flag) {
    return static_cast<PhysicsAttributeFlags>(~static_cast<uint32_t>(flag));
}