#include "ReflectionSubsystem.h"

namespace Denix
{
    void ReflectionSubsystem::Initialize()
    {
        DE_LOG(LogReflection, Warn, "Reflection Subsystem Initializing")
        Subsystem::Initialize();
        DE_LOG(LogReflection, Info, "Reflection Subsystem Initialized")
    }

    void ReflectionSubsystem::Deinitialize()
    {
        DE_LOG(LogReflection, Trace, "Reflection Subsystem Deinitializing")
        Subsystem::Deinitialize();
        DE_LOG(LogReflection, Trace, "Reflection Subsystem Denitialized")
    }
}
