#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "ProfileSession.h"

#include "Denix/Thread/JobProfile.h"

void Denix::ProfileSession::StartProfile(const std::string& _name)
{
    Profile& profile = GetProfile(_name);
    profile.Start();
}

void Denix::ProfileSession::EndProfile(const std::string& _name)
{
    Profile& profile = GetProfile(_name);
    profile.End();
}

void Denix::ProfileSession::StartJobProfile(const Ref<JobDeclaration>& _job)
{
    JobProfile& jobProfile = GetJobProfile(_job->m_Name);
    jobProfile.Start();
}

void Denix::ProfileSession::EndJobProfile(const Ref<JobDeclaration>& _job)
{
    JobProfile& jobProfile = GetJobProfile(_job->m_Name);
    jobProfile.End();
}

Denix::Profile& Denix::ProfileSession::GetProfile(const std::string& _name)
{
    // Create profile if it doesn't exist. Occurs on first instance of profile
    if (!m_InlineProfiles.contains(_name))
    {
        m_InlineProfiles[_name] = Profile({_name});
        DE_LOG(LogProfile, Trace, "Profile {} created", _name)
    }
    
    return m_InlineProfiles[_name];
}

Denix::JobProfile& Denix::ProfileSession::GetJobProfile(const std::string& _name)
{
    // Create job profile if it doesn't exist. Occurs on first instance of profile
    if (!m_JobProfiles.contains(_name))
    {
        m_JobProfiles[_name] = JobProfile({_name});
        DE_LOG(LogProfile, Trace, "Job Profile {} created", _name)
    }

    return m_JobProfiles[_name];
}