#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "ProfileSubsystem.h"

#include "Denix/Core/TimerSubsystem.h"

namespace Denix
{
    ProfileSubsystem* ProfileSubsystem::s_ProfileSubsystem = nullptr;
    
    ProfileSubsystem::ProfileSubsystem()
    {
        s_ProfileSubsystem = this;
        DE_LOG_CREATE(LogProfile)
    }

    ProfileSubsystem::~ProfileSubsystem()
    {
        s_ProfileSubsystem = nullptr;
    }

    void ProfileSubsystem::Initialize()
    {
        Subsystem::Initialize();

        DE_LOG(LogProfile, Warn, "Profile Subsystem Initializing")

        // Get the engine timer profile
        if(const Ref<Profile>& timerProfile = TimerSubsystem::Get()->m_EngineProfile)
        {
            m_Profiles[timerProfile->GetName()] = timerProfile;
            DE_LOG(LogProfile, Info, "Added Timer Profile to Profile Subsystem")
        }
        else
        {
            throw std::runtime_error("Timer Profile not found in Timer Subsystem");
        }

        DE_LOG(LogProfile, Info, "Profile Subsystem Initialized")
    }

    void ProfileSubsystem::Deinitialize()
    {
        DE_LOG(LogProfile, Trace, "Profile Subsystem Deinitialized")
        Subsystem::Deinitialize();
    }

    void ProfileSubsystem::Update(float _deltaTime)
    {
        Subsystem::Update(_deltaTime);

        for (const auto& profile : m_Profiles | std::views::values)
        {
            profile->m_FramePercentage = profile->m_AverageDuration / _deltaTime;
        }
    }

    void ProfileSubsystem::StartProfile(const std::string& _name)
    {
        // Check if profile exists - This usually only happens when the profile is created in the DE_PROFILE macro
        if (!s_ProfileSubsystem->m_Profiles.contains(_name))
        {
           s_ProfileSubsystem-> m_Profiles[_name] = MakeRef<Profile>(ObjectInit(_name));
        }

        if(const Ref<Profile>& profile = s_ProfileSubsystem->m_Profiles[_name])
        {
            profile->Start();
        }
    }

    void ProfileSubsystem::EndProfile(const std::string& _name)
    {
        // Check if profile exists. We don't want to create a new profile if it doesn't exist.
        if(!s_ProfileSubsystem->m_Profiles.contains(_name))
        {
            DE_LOG(LogProfile, Error, "Profile does not exist: {}", _name.c_str())
            return;
        }

        if(const Ref<Profile>& profile = s_ProfileSubsystem->m_Profiles[_name])
        {
            profile->End();
        }
    }
}
