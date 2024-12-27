#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "Profile.h"

#include <algorithm>

namespace Denix
{
    Profile::Profile(const ObjectInit& _objInit): Object(_objInit)
    {
        m_Timer = MakeRef<Timer>();
        m_AverageDuration = 0.0f;
        m_MinimumDuration = 0.0f;
        m_MaximumDuration = 0.0f;
    }

    void Profile::Start()
    {
        m_Timer->Start();
    }

    void Profile::End()
    {
        m_Timer->Stop();
        const float durationMs = m_Timer->GetDurationMs();
        
        // Record the duration
        m_DurationBuffer.SaveResult(m_Timer->m_TimeEvent);
        
        // Update the minimum duration. Minimum duration equals 0.0f if it hasn't been set yet
        if (durationMs < m_MinimumDuration || m_MinimumDuration == 0.0f)  m_MinimumDuration = durationMs;

        // Update the maximum duration
        m_MaximumDuration = std::max(durationMs, m_MaximumDuration);

        // Calculate the average duration. We average the last m_AverageDurationCount durations
        if (m_DurationBuffer.ProfileResults.size() < s_AverageDurationCount) return;

        float durationSum = 0.0f;
        int count = 0;
        for (const TimeEvent data : std::ranges::views::reverse(m_DurationBuffer.ProfileResults))
        {
            durationSum += data.Duration;
            if (count++ >= s_AverageDurationCount) break;
        }
        m_AverageDuration = durationSum / static_cast<float>(s_AverageDurationCount);
    }
}
