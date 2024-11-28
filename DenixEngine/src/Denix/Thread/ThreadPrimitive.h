#pragma once
#include <functional>

#include "Denix/Core.h"
#include "Denix/Core/Timer.h"

namespace  Denix
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
        Counter(int _value) : m_Value(_value) {}
        void Increment()
        {
            m_Value.fetch_add(1);
        }

        void Decrement()
        {
            m_Value.fetch_sub(1);
            //m_ConditionVar.notify_all();
        }

        void Add(int _value)
        {
            m_Value.fetch_add(_value);
        }

        void Subtract(int _value)
        {
            m_Value.fetch_sub(_value);
        }
        
        std::atomic_int m_Value{0};
        //std::mutex m_Mutex;
       // std::condition_variable m_ConditionVar;
    };
    
    struct JobDeclaration
    {
        JobDeclaration() = default;
        
        template <typename Func>
        JobDeclaration(std::string _name, Priority _priority, const Ref<Counter>& _waitCounter, Func&& _entryPoint)
            : m_Name(std::move(_name)), m_WaitCounter(_waitCounter), m_EntryPoint(std::forward<Func>(_entryPoint)), m_Priority(_priority)
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
