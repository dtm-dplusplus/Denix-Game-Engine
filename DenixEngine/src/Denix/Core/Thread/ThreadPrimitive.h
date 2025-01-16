#pragma once
#include "Denix/Core.h"
#include "Denix/Core/Time/TimePrimitive.h"

namespace Denix
{
    /**
     * @brief Job priority enum
     * Higher priority jobs will be executed before lower priority jobs
     */
    enum class Priority: uint8_t
    {
        LATENT, // Job can be executed at any time
        NORMAL, // Job should be executed in a timely manner
        HIGH, // Job should be executed as soon as possible
        CRITICAL // Job must be executed immediately
    };

    /**
     * @brief Counter struct to manage job dependencies
     *  Used to keep track of how many jobs are dependent on a specific job
     *  Lightweight alternative to direct job dependencies
     */
    struct Counter
    {
        Counter() = default;

        /**
         * @brief Increment the counter
         * Symbolises that a job is waiting on this counter
         */
        void Increment() {++m_Value;}

        /**
         * @brief Decrement the counter
         * Symbolises that a job is no longer waiting on this counter
         */
        void Decrement() {--m_Value;}

        /**
         * @brief Thread Safe atomic counter value
         * This is the value that is incremented and decremented
         * Zero means no jobs are waiting on this counter
         */
        std::atomic_int m_Value{0};
    };

    /**
     * @brief Mutex struct to manage thread locking
     */
    struct Mutex
    {
        Mutex() = default;

        /**
         * @brief Lock the mutex
         */
        void Lock() {m_Lock = true;}

        /**
         * @brief Unlock the mutex
         */
        void Unlock() {m_Lock = false;}

        /**
         * @brief Check if the mutex is locked
         * @return True if the mutex is locked, false otherwise
         */
        bool IsLocked() const {return m_Lock;} 
        
        std::atomic_bool m_Lock{false};
    };

    /*struct Lock
    {
        Lock(Mutex& _mutex): m_Mutex(_mutex)
        {
            m_Mutex.Lock();
        }

        ~Lock()
        {
        }

        Mutex& m_Mutex;
    };*/

    struct LockGuard
    {
        LockGuard(Mutex& _mutex): m_Mutex(_mutex)
        {
            while (m_Mutex.IsLocked())
            {
                // Spin lock
            }
            m_Mutex.Lock();
        }

        ~LockGuard()
        {
            m_Mutex.Unlock();
        }

        Mutex& m_Mutex;
    };
    
    /**
     * @brief This struct is used to declare a job to the job subsystem
     * It contains all the information needed to execute a job
     * Profile information is also stored here
     */
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
         * @brief Jobs entry point function. Contains the code to be executed and any arguments
         */
        std::function<void()> m_EntryPoint;

        /**
         * @brief Job priority. Higher priority jobs will be executed before lower priority jobs
         */
        Priority m_Priority;

        /**
         * @brief Counter to manage job dependencies
         */
        Ref<Counter> m_WaitCounter;

        /**
         * @brief Time event used to profile the job
         */
        TimeEvent m_JobTime;

        /**
         * @brief Index of the thread that executed the job for debugging
         */
        int m_ThreadIndex;
    };
    
}
