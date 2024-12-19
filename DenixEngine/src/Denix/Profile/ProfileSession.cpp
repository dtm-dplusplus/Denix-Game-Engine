#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "ProfileSession.h"

#include "Denix/Thread/JobProfile.h"

void Denix::ProfileSession::StartSession()
{
    m_IsProfiling = true;
    m_SessionTimer->Start();
}

void Denix::ProfileSession::EndSession()
{
    m_SessionTimer->Stop();
    m_IsProfiling = false;
}

void Denix::ProfileSession::StartInlineProfile(const std::string& _name)
{
    Profile& profile = GetInlineProfile(_name);
    profile.Start();
}

void Denix::ProfileSession::EndInlineProfile(const std::string& _name)
{
    Profile& profile = GetInlineProfile(_name);
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
    
    //jobProfile.m_JobBuffer.SaveJobResult(_job);
    

    // Calculate the average duration. We average the last m_AverageDurationCount durations
    /*if (jobProfile.m_JobBuffer.JobResults.size() < Profile::s_AverageDurationCount) return;

    float durationSum = 0.0f;
    int count = 0;
    for (const auto& jobData : std::ranges::views::reverse(jobProfile.m_JobBuffer.JobResults))
    {
        durationSum += jobData->m_JobTime.Duration;
        if (count++ >= Profile::s_AverageDurationCount) break;
    }
    jobProfile.m_AverageDuration = durationSum / static_cast<float>(Profile::s_AverageDurationCount);*/
}

Denix::Profile& Denix::ProfileSession::GetInlineProfile(const std::string& _name)
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