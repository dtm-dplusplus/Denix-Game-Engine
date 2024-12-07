#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "JobProfile.h"
#include "JobDecleration.h"

void Denix::JobProfile::End()
{
    m_Timer->Stop();

    if (m_Jobs.back()->m_ThreadIndex > -1) m_LastThreadIndex = m_Jobs.back()->m_ThreadIndex;
    // Record the duration
    float  duration = m_Timer->GetDuration() * 1000.0f;
    m_DurationRecords.push_back(duration);
        
    // Update the minimum and maximum durations
    if (m_MinimumDuration == 0.0f) m_MinimumDuration = duration;
    else m_MinimumDuration = std::min(duration, m_MinimumDuration);

    m_MaximumDuration = std::max(duration, m_MaximumDuration);

    float durationSum = [&] { float sum =0.0f; for(const float d: m_DurationRecords) sum += d; return sum; }();
    m_AverageDuration = durationSum / static_cast<float>(m_DurationRecords.size());
    m_ExecTime = duration;
}
