#pragma once

#include <cstdint>

// Define bit flags for physics attributes
enum PhysicsAttributeFlags
{
    PHYSICS_SIMULATE     = (1 << 0)    // 0001 - Position modified
   //PHYSICS_VELOCITY     = 1 << 1,     // 0010 - Velocity modified
   //PHYSICS_ROTATION     = 1 << 2,     // 0100 - Rotation modified
   //PHYSICS_SCALE        = 1 << 3,     // 1000 - Scale modified
   //PHYSICS_FORCE        = 1 << 4,     // 10000 - Force applied
   //PHYSICS_COLLISION    = 1 << 5,     // 100000 - Collision state changed
   //PHYSICS_CONSTRAINT   = 1 << 6,     // 1000000 - Constraints modified
   // PHYSICS_ALL          = 0xFFFFFFFF  // All attributes modified
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