#pragma once

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
    void Increment() { ++m_Value; }

    /**
     * @brief Decrement the counter
     * Symbolises that a job is no longer waiting on this counter
     */
    void Decrement() { --m_Value; }

    /**
     * @brief Thread Safe atomic counter value
     * This is the value that is incremented and decremented
     * Zero means no jobs are waiting on this counter
     */
    std::atomic_int m_Value{0};
};
