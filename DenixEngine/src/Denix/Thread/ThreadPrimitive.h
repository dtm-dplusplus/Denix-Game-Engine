#pragma once
#include <functional>

#include "Denix/Core.h"
#include "Denix/Core/Timer.h"

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

    struct JobDeclaration
    {
        JobDeclaration() = default;

        JobDeclaration(const std::string& name, Priority priority, Ref<Counter> waitCounter,
                       std::function<void()> entryPoint)
            : m_Name(name), m_EntryPoint(std::move(entryPoint)), m_Priority(priority),
              m_WaitCounter(std::move(waitCounter)), m_Timer(ObjectInit(name))
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

        /*template <typename... Args>
        std::tuple<Args...> m_Args;*/

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
        Timer m_Timer;
    };
}
