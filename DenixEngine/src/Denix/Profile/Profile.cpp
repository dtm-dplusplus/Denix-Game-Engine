#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "Profile.h"

#include "Denix/Core/TimerSubsystem.h"

namespace Denix
{
    Profile::Profile(const ObjectInitializer& _objInit): Object(_objInit)
    {
        m_Timer = MakeRef<Timer>(_objInit);
        m_DurationCount = 0;
        m_AverageDuration = 0.0f;
        m_MinimumDuration = 0.0f;
        m_MaximumDuration = 0.0f;
        m_AverageDurationCount = 60;
        m_DurationRecords.assign(m_AverageDurationCount, 0.0f);
    }

    void Profile::Start()
    {
        m_Timer->Start();
    }

    void Profile::End()
    {
        m_Timer->Stop();

        // Record the duration
        m_DurationRecords[m_DurationCount] = m_Timer->GetDuration<std::milli>();
        m_Buffer.AddPoint(TimerSubsystem::GetProgramElapsedTime(),  m_DurationRecords[m_DurationCount]);
        
        // Update the minimum and maximum durations
        if (m_DurationRecords[m_DurationCount] < m_MinimumDuration || m_MinimumDuration == 0.0f)
            m_MinimumDuration = m_DurationRecords[m_DurationCount];

        if (m_DurationRecords[m_DurationCount] > m_MaximumDuration)
            m_MaximumDuration = m_DurationRecords[m_DurationCount];
        
        m_DurationCount++;
        // Calculate the average duration
        if (m_DurationCount >= m_AverageDurationCount)
        {
            float durationSum = [&] { float sum =0.0f; for(int i = 0; i < m_AverageDurationCount; i++) sum += m_DurationRecords[i]; return sum; }();
            m_AverageDuration = durationSum / static_cast<float>(m_DurationCount);

            // Reset the duration count
            m_DurationCount = 0;
        }
    }

    float Profile::GetDuration() const
    {
        return m_DurationRecords[m_DurationCount];
    }
}
