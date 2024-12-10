#pragma once
#include "Denix/Core.h"
#include "Denix/Core/TimePrimitive.h"

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
        }

        std::atomic_int m_Value{0};
    };

    struct JobDeclaration
    {
        JobDeclaration(std::string _name, const Priority _priority, Ref<Counter> _waitCounter,
                       std::function<void()> _entryPoint):
            m_Name(std::move(
                _name)), m_EntryPoint(std::move(_entryPoint)),
            m_Priority(_priority),
            m_WaitCounter(std::move(_waitCounter)), m_ThreadIndex(-1)
        {
        }

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

        TimeEvent m_JobTime;

        int m_ThreadIndex;
    };
    
}
