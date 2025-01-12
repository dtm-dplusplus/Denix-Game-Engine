#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "ProfileSession.h"

#include <algorithm>
#include <numeric>

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

    // Calculate thread data for visualization
    for (auto thread: m_ThreadData)
    {
        m_ThreadJobCounts.push_back(thread.m_JobExecCount);
        m_ThreadJobExecTimes.push_back(thread.m_ThreadExecTime);
        m_ThreadSleepTimes.push_back(thread.m_ThreadSleepTime);
    }

    // Calculate session frame times
    for (const auto& frameTime: m_FrameTimes)
    {
        m_TotalFrameTime += frameTime;
        if (frameTime < m_MinFrameTime || m_MinFrameTime == 0.0f) m_MinFrameTime = frameTime;
        m_MaxFrameTime = std::max(frameTime, m_MaxFrameTime);
    }

    m_MinFrameTime*= 1000.0f;
    m_MaxFrameTime*= 1000.0f;
    m_AverageFrameTime = m_TotalFrameTime / static_cast<float>(m_FrameTimes.size());

    // Calculate session frames per second
    m_AverageFramesPerSecond = std::accumulate(m_FramesPerSeconds.begin(), m_FramesPerSeconds.end(), 0) / m_FramesPerSeconds.size();

    // Flush the frame times
    m_FrameTimes.clear();
    m_FramesPerSeconds.clear();

    // Signal the end of the session
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
    _job->m_JobTime = jobProfile->GetLastProfileResult();
}

Ref<Profile> ProfileSession::GetInlineProfile(const std::string& _name)
{
    // Create profile if it doesn't exist. Occurs on first instance of profile
    if (!m_InlineProfileMap.contains(_name))
    {
        m_InlineProfiles.emplace_back(MakeRef<Profile>(_name));
        m_InlineProfileMap[_name] = m_InlineProfiles.back();
        //DE_LOG(LogProfile, Trace, "Profile {} created", _name)
    }
    
    return m_InlineProfileMap[_name];
}

Ref<JobProfile> ProfileSession::GetJobProfile(const std::string& _name)
{
    // Create job profile if it doesn't exist. Occurs on first instance of profile
    if (!m_JobProfilesMap.contains(_name))
    {
        m_JobProfiles.emplace_back(MakeRef<JobProfile>(_name));
        m_JobProfilesMap[_name] = m_JobProfiles.back();
        //DE_LOG(LogProfile, Trace, "Job Profile {} created", _name)
    }

    return m_JobProfilesMap[_name];
}