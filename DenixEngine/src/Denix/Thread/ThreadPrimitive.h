#pragma once

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
    
}
