#include "ReflectionSubsystem.h"

namespace Denix
{
    void ReflectionSubsystem::Initialize()
    {
        Subsystem::Initialize();
        DE_LOG(LogReflection, Warn, "Reflection Subsystem Initializing")
        DE_LOG(LogReflection, Info, "Reflection Subsystem Initialized")
    }

    void ReflectionSubsystem::Deinitialize()
    {
        Subsystem::Deinitialize();
        DE_LOG(LogReflection, Trace, "Reflection Subsystem Denitialized")
    }
}