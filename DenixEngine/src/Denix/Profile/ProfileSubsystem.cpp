#include "ProfileSubsystem.h"

#include "Denix/Core/Time/TimerSubsystem.h"
#include "Denix/Core/Thread/JobSubsystem.h"

namespace Denix
{
    void ProfileSubsystem::Initialize()
    {
        DE_LOG(LogProfile, Warn, "Profile Subsystem Initializing")
        Subsystem::Initialize();

        DE_LOG(LogProfile, Info, "Profile Subsystem Initialized")
    }

    void ProfileSubsystem::Deinitialize()
    {
        DE_LOG(LogProfile, Trace, "Profile Subsystem Deinitializing")
        m_ProfileSessions.clear();
        m_ActiveProfileSession.reset();
        Subsystem::Deinitialize();
        DE_LOG(LogProfile, Trace, "Profile Subsystem Deinitialized")
    }

    void ProfileSubsystem::StartProfileSession(const std::string& _name)
    {
        if (s_Instance->m_ActiveProfileSession)
        {
            DE_LOG(LogProfile, Warn, "Profile Session already active")
            return;
        }

        s_Instance->m_ProfileSessions.emplace_back(
            MakeRef<ProfileSession>(ObjectInit(std::to_string(s_Instance->m_ProfileSessions.size()) + "_" + _name)));
        s_Instance->m_ActiveProfileSession = s_Instance->m_ProfileSessions.back();
        s_Instance->m_ActiveProfileSession->StartSession();

        JobSubsystem::StartThreadProfiling();
        DE_LOG(LogProfile, Info, "Profile Session Started: {}", s_Instance->m_ActiveProfileSession->GetName())
    }

    void ProfileSubsystem::EndProfileSession()
    {
        if (!s_Instance->m_ActiveProfileSession)
        {
            DE_LOG(LogProfile, Warn, "No active Profile Session to end")
            return;
        }

        DE_LOG(LogProfile, Info, "Profile Session Ended: {}", s_Instance->m_ActiveProfileSession->GetName())

        // Do end of session processing
        JobSubsystem::StopThreadProfiling();
        s_Instance->m_ActiveProfileSession->EndSession();

        // clear active session
        s_Instance->m_ActiveProfileSession = nullptr;
    }

    void ProfileSubsystem::StartInlineProfile(const std::string& _name)
    {
        // Check if we have an active profile session to record the profile
        if (!s_Instance->m_ActiveProfileSession) return;

        s_Instance->m_ActiveProfileSession->StartInlineProfile(_name);
    }

    void ProfileSubsystem::EndInlineProfile(const std::string& _name)
    {
        // Check if we have an active profile session to record the profile
        if (!s_Instance->m_ActiveProfileSession) return;

        s_Instance->m_ActiveProfileSession->EndInlineProfile(_name);
    }

    void ProfileSubsystem::StartJobProfile(const Ref<JobDeclaration>& _job)
    {
        // Check if we have an active profile session to record the profile
        if (!s_Instance->m_ActiveProfileSession) return;

        s_Instance->m_ActiveProfileSession->StartJobProfile(_job);
    }

    void ProfileSubsystem::EndJobProfile(const Ref<JobDeclaration>& _job)
    {
        // Check if we have an active profile session to record the profile
        if (!s_Instance->m_ActiveProfileSession) return;

        s_Instance->m_ActiveProfileSession->EndJobProfile(_job);
    }
}
