#pragma once
#include "Denix/Core.h"
#include <thread>
#include <functional>
#include <source_location>
#include <queue>
#include <utility>

#include <windows.h>

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
        Counter(int _value) : m_Value(_value) {}
        void Increment()
        {
            m_Value.fetch_add(1);
        }

        void Decrement()
        {
            m_Value.fetch_sub(1);
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
        std::mutex m_Mutex;
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
    
    class Thread
    {
    public:
        Thread(): m_Job()
        {
            m_IsWorking = false;
        }

        ~Thread()
        {
            DE_LOG(Log, Info, "Thread: {} destroyed", m_ThreadIDInt)
            JoinCheck();
        }

        template <typename Func, typename... Args>
        void InitThread(Func&& _func, Args&&... _args)
        {
            // = std::bind(std::forward<Func>(_func), std::forward<Args>(_args)...); //Lambda here?
            m_Thread = std::thread(std::forward<Func>(_func), std::forward<Args>(_args)...);
            m_ThreadID = m_Thread.get_id();
            SetThreadIDInt();
            DE_LOG(Log, Info, "Thread: {} created", m_ThreadIDInt)
        }

        void InitWorkerThread()
        {
            m_Thread = std::thread(&Thread::Work, this);
            m_ThreadID = m_Thread.get_id();
            SetThreadIDInt();
            DE_LOG(Log, Info, "Thread: {} created", m_ThreadIDInt)
        }

        // Delete copy constructor and copy assignment operator
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        // Allow move constructor and move assignment operator
        Thread(Thread&&) = default;
        Thread& operator=(Thread&&) = default;

        void Work()
        {
            while (true)
            {
                if (m_Job.m_EntryPoint)
                {
                    DE_LOG(Log, Info, "Thread: {} Priority: {} Job: {}", m_ThreadIDInt, PriorityToString(m_Job.m_Priority), m_Job.m_Name)
                    m_IsWorking = true;
                    m_Job.m_Timer.Start();
                    m_Job.m_EntryPoint();
                    m_Job.m_Timer.Stop();
                    m_Job.m_EntryPoint = nullptr;

                    // Decrement Counter
                    assert(m_Job.m_WaitCounter);
                    m_Job.m_WaitCounter->Decrement();

                    // Let the scheduler know that the job is done
                    m_JobsDone++;
                    m_IsWorking = false;
                    DE_LOG(Log, Info, "Thread {} Priority: {} Job Done: {}", m_ThreadIDInt, PriorityToString(m_Job.m_Priority), m_Job.m_Name)
                }
            }
        }


        /**
         * @brief Join the thread. 
         */
        void Join()
        {
            if (m_Thread.joinable())
            {
                m_Thread.join();
                m_IsWorking = false;
                DE_LOG(Log, Info, "Thread {} joined", m_ThreadIDInt)
            }
        }

        /**
         * @brief Join the thread. Check if the thread is joinable before joining
         * Automatically called in the destructor
         */
        void JoinCheck()
        {
            if (m_Thread.joinable())
            {
                m_Thread.join();
                m_IsWorking = false;
                DE_LOG(Log, Info, "Thread {} joined", m_ThreadIDInt)
            }
            else
            {
                DE_LOG(Log, Error, "Thread {} not joinable", m_ThreadIDInt)
            }
        }

        bool IsJoinable() const
        {
            return m_Thread.joinable();
        }

        /**
         * @brief Detach the thread from the main thread.
         *  Allowis the thread to continue execution independently.
         *  Once detached, the thread becomes non-joinable, meaning you cannot wait for it to finish using join.
         *  The detached thread will run until it completes, and its resources will be released automatically.
         */
        void Detach()
        {
            if (m_Thread.joinable())
            {
                m_Thread.detach();
                DE_LOG(Log, Info, "Thread {} detached", m_ThreadIDInt)
            }
            else
            {
                DE_LOG(Log, Error, "Thread {} failed to detach. Not joinable", m_ThreadIDInt)
            }
        }

        void SetThreadIDInt()
        {
            std::stringstream ss;
            ss << m_Thread.get_id();
            m_ThreadIDInt = std::stoi(ss.str());
        }

        // private:
        std::thread m_Thread;
        std::thread::id m_ThreadID;
        size_t m_ThreadIDInt;

        JobDeclaration m_Job;
        int m_JobsDone = 0;

        bool m_IsWorking;
        bool m_StopFlag = false;

        friend class JobSubsystem;
    };

    inline void WaitForCounter(Counter& _counter)
    {
        // Wait until the counter reaches the target value
       if (_counter.m_Value.load() == 0) return;
        _counter.m_Mutex.lock();
        while (_counter.m_Value.load() > 0)
        {
            _counter.m_Mutex.unlock();
            std::this_thread::yield();
            _counter.m_Mutex.lock();
        }
    }

  
}
