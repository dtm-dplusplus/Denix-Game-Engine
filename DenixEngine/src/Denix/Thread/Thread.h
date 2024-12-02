#pragma once
#include "Denix/Core.h"
#include <thread>
#include <functional>


#include "Denix/Core/Timer.h"
#include "Denix/Thread/ThreadPrimitive.h"

namespace Denix
{
  
    
    class Thread
    {
    public:
        Thread()
        {
            m_ShouldWork = true;
            m_ThreadIDInt = 0;
            m_JobExecCount = 0;
        }

        ~Thread()
        {
           m_ShouldWork = false;
           JoinCheck();
        }

        template <typename Func, typename... Args>
        void InitThread(Func&& _func, Args&&... _args)
        {
            // = std::bind(std::forward<Func>(_func), std::forward<Args>(_args)...); //Lambda here?
            m_Thread = std::thread(std::forward<Func>(_func), std::forward<Args>(_args)...);
            m_ThreadID = m_Thread.get_id();
            SetThreadIDInt();
            DE_LOG(LogThread, Info, "Thread: {} created", m_ThreadIDInt)
        }

        void InitWorkerThread()
        {
            m_Thread = std::thread(&Thread::Work, this);
            m_ThreadID = m_Thread.get_id();
            SetThreadIDInt();
            DE_LOG(LogThread, Info, "Thread: {} created", m_ThreadIDInt)
        }

        // Delete copy constructor and copy assignment operator
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;

        // Allow move constructor and move assignment operator
        Thread(Thread&&) = default;
        Thread& operator=(Thread&&) = default;

        void Work();


        /**
         * @brief Join the thread. 
         */
        void Join()
        {
            m_Thread.join();
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
                DE_LOG(LogThread, Trace, "Thread {} joined", m_ThreadIDInt)
            }
            else
            {
                DE_LOG(LogThread, Error, "Thread {} not joinable", m_ThreadIDInt)
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
                DE_LOG(LogThread, Info, "Thread {} detached", m_ThreadIDInt)
            }
            else
            {
                DE_LOG(LogThread, Error, "Thread {} failed to detach. Not joinable", m_ThreadIDInt)
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
        std::mutex m_Mutex;
        
        Ref<JobDeclaration> m_Job;

        
        
        bool m_ShouldWork;

        static int s_WaitForCounterSleepTime;

        
        static int s_WaitForJobSleepTime;


        // Thread Profiling - These are not thread safe and should be managed by the JobSubsystem
        /**
         * @brief Should the thread profile itself. Global setting
         */
        static bool s_ShouldProfile;

        
        /**
         * @brief Number of jobs executed by the thread
         */
        size_t m_JobExecCount;
        
        /**
         * @brief Total time the thread spent executing jobs
         */
        float m_ThreadExecTime;

        /**
         * @brief Total time the thread spent sleeping. Only accounts for time in the Work() function
         */
        float m_ThreadSleepTime;

        friend class JobSubsystem;
    };

    inline void WaitForCounter(const Counter* _counter)
    {
        if (!_counter) return;

        // Wait for the counter to reach zero, then continue. Sleep to reduce CPU usage
        while (_counter->m_Value > 0)
        {
            // Add sleep condition here to reduce CPU usage
            std::this_thread::sleep_for(std::chrono::nanoseconds(Thread::s_WaitForCounterSleepTime));
        }
    }
}