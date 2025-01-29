#include "JobProfile.h"

void Denix::JobProfile::End()
{
    Profile::End();

    /*m_Timer->Stop();

    // Record the duration
    float  duration = m_Timer->GetDuration() * 1000.0f;
        
    // Update the minimum and maximum durations
    if (m_MinimumDuration == 0.0f) m_MinimumDuration = duration;
    else m_MinimumDuration = std::min(duration, m_MinimumDuration);

    m_MaximumDuration = std::max(duration, m_MaximumDuration);*/
}
