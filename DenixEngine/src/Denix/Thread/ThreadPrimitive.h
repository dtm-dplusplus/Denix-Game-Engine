#pragma once
#include <algorithm>
#include <functional>

#include "Denix/Core.h"
#include "Denix/Core/Timer.h"
#include "Denix/Profile/Profile.h"

namespace Denix
{
    enum class Priority
    {
        LATENT, NORMAL, HIGH, CRITICAL
    };

    inline std::string PriorityToString(const Priority _priority)
    {
        switch (_priority)
        {
        case Priority::LATENT: return "LATENT";
        case Priority::NORMAL: return "NORMAL";
        case Priority::HIGH: return "HIGH";
        case Priority::CRITICAL: return "CRITICAL";
        }
        return "UNKNOWN";
    }

    /**
     * @brief Counter struct to manage job dependencies
     *  Used to keep track of how many jobs are dependent on a specific job
     *  Lightweight alternative to direct job dependencies
     */
    struct Counter
    {
        Counter() = default;

        Counter(int _value) : m_Value(_value)
        {
        }

        void Increment()
        {
            ++m_Value;
        }

        void Decrement()
        {
            --m_Value;
            //m_ConditionVar.notify_all();
        }

        std::atomic_int m_Value{0};
        //std::mutex m_Mutex;
        // std::condition_variable m_ConditionVar;
    };

    class JobProfile: public Profile
    {
    public:
        JobProfile(const ObjectInit& _objInit)
            : Profile(_objInit)
        {
        }

        void End() override
        {
            m_Timer->Stop();

            // Record the duration
            float  duration = m_Timer->GetDuration() * 1000.0f;
            m_DurationRecords.push_back(duration);
        
            // Update the minimum and maximum durations
            if (m_MinimumDuration == 0.0f) m_MinimumDuration = duration;
            else m_MinimumDuration = std::min(duration, m_MinimumDuration);

            m_MaximumDuration = std::max(duration, m_MaximumDuration);

            float durationSum = [&] { float sum =0.0f; for(const float d: m_DurationRecords) sum += d; return sum; }();
            m_AverageDuration = durationSum / static_cast<float>(m_DurationRecords.size());
        }
    };
    
    struct JobDeclaration
    {
        JobDeclaration(const std::string& _name, const Priority _priority, Ref<Counter> _waitCounter, const Ref<JobProfile>& _profile,
                       std::function<void()> _entryPoint)
            : m_ID(s_IDCounter++), m_Name(_name), m_EntryPoint(std::move(_entryPoint)),
              m_Priority(_priority),
              m_WaitCounter(std::move(_waitCounter)), m_JobProfile(_profile)
        {
        }

        size_t m_ID;

       // size_t m_FrameID;

        /**
         * @brief Job name. Used for debugging purposes
         */
        std::string m_Name;

        /**
         * @brief Jobs entry point function
         */
        std::function<void()> m_EntryPoint;

        /**
         * @brief Job priority
         */
        Priority m_Priority;

        /**
         * @brief Counter to keep track of how many jobs we are waiting on before a job group is finished.
         * Should probably be moved to some kind of job buidler which sets up dependencies
         */
        Ref<Counter> m_WaitCounter;

        /**
         * @brief Timer to keep track of how long the job takes to execute
        */
        Ref<Profile> m_JobProfile;


       inline static size_t s_IDCounter = 0;
    };
}
