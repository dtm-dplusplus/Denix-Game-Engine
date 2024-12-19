#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "ProfileSession.h"

#include "Denix/Thread/JobProfile.h"

using namespace Denix;

void ProfileSession::StartSession()
{
    m_IsProfiling = true;
    m_SessionTimer->Start();
}

void ProfileSession::EndSession()
{
    m_SessionTimer->Stop();
    m_IsProfiling = false;
}

void ProfileSession::StartInlineProfile(const std::string& _name)
{
    Ref<Profile> profile = GetInlineProfile(_name);
    profile->Start();
}

void ProfileSession::EndInlineProfile(const std::string& _name)
{
    Ref<Profile> profile = GetInlineProfile(_name);
    profile->End();
}

void ProfileSession::StartJobProfile(const Ref<JobDeclaration>& _job)
{
    Ref<JobProfile> jobProfile = GetJobProfile(_job->m_Name);
    jobProfile->Start();
}

void ProfileSession::EndJobProfile(const Ref<JobDeclaration>& _job)
{
    Ref<JobProfile> jobProfile = GetJobProfile(_job->m_Name);
    jobProfile->End();
}

Ref<Profile> ProfileSession::GetInlineProfile(const std::string& _name)
{
    // Create profile if it doesn't exist. Occurs on first instance of profile
    if (!m_InlineProfileMap.contains(_name))
    {
        m_InlineProfiles.push_back(MakeRef<Profile>(_name));
        m_InlineProfileMap[_name] = m_InlineProfiles.back();
        DE_LOG(LogProfile, Trace, "Profile {} created", _name)
    }
    
    return m_InlineProfileMap[_name];
}

Ref<JobProfile> ProfileSession::GetJobProfile(const std::string& _name)
{
    // Create job profile if it doesn't exist. Occurs on first instance of profile
    if (!m_JobProfilesMap.contains(_name))
    {
        m_JobProfiles.push_back(MakeRef<JobProfile>(_name));
        m_JobProfilesMap[_name] = m_JobProfiles.back();
        DE_LOG(LogProfile, Trace, "Job Profile {} created", _name)
    }

    return m_JobProfilesMap[_name];
}