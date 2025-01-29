#pragma once

#include "Denix/Core/Thread/ThreadPrimitive.h"
#include <thread>

namespace Denix
{
    struct JobDeclaration;

    class Thread
    {
    public:
        explicit Thread(int _index);

        ~Thread();

        // Delete copy constructor and copy assignment operator
        Thread(const Thread& other) = delete;
        Thread(Thread&& other) noexcept = delete;
        Thread& operator=(const Thread& other) = delete;
        Thread& operator=(Thread&& other) noexcept = delete;

        static int& GetWaitForCounterSleepTime() { return s_WaitForCounterSleepTime; }
        static int& GetWaitForJobSleepTime() { return s_WaitForJobSleepTime; }

    private:
        /**
         * @brief The main function of the thread
         * This function is called when the thread is created and will run until the thread is destroyed
         */
        void Work();

        /**
         * @brief The actual software thread this class wraps
         */
        std::thread m_Thread;


        /**
         * @brief The ID of the thread. Used for debugging
         * Matches the ID available in the debugger
         */
        size_t m_ThreadID;

        /**
         * @brief Index of the thread in the thread pool
         */
        int m_ThreadIndex;

        /**
         * @brief The job the thread is currently executing
         */
        Ref<JobDeclaration> m_Job;

        /**
         * @brief Determines if the thread should work
         *  This is managed by the JobSubsystem
         */
        bool m_ShouldWork;

        /**
         * @brief Determines the lifetime of the thread. If false, the thread will exit
         */
        bool m_Active;

        inline static int s_WaitForCounterSleepTime = 1;


        /**
         * @brief Value used between jobs to allow job queue to populate and prioritize
         * Also used to reduce CPU usage
         */
        inline static int s_WaitForJobSleepTime = 110;


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

        /**
         * @brief Should the thread profile itself. Global setting managed by the JobSubsystem
         */
        inline static bool s_ShouldProfile = false;

        friend class JobSubsystem;
        friend void WaitForCounter(const Ref<Counter>& _counter);
    };


    /**
     * Utility to synchronize Job execution & Dependencies
     * @param _counter Wait Counter to synchronize with
     */
    inline void WaitForCounter(const Ref<Counter>& _counter)
    {
        if (!_counter) return;

        // Wait for the counter to reach zero, then continue.
        // Add sleep condition here to reduce CPU usage & give other threads time to work
        // In the future we can switch to a more efficient wait condition
        while (_counter->m_Value > 0)
            std::this_thread::sleep_for(std::chrono::nanoseconds(Thread::s_WaitForCounterSleepTime));
    }
}
