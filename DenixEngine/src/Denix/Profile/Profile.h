#pragma once

#include "Denix/Scene/Object.h"
#include "Denix/Core/Timer.h"

namespace Denix
{
    class Timer;

    struct DurationBuffer
    {
        std::vector<TimeEvent> ProfileResults;

        DurationBuffer()
        {
            // Reserve space to reduce reallocations during profiling
            ProfileResults.reserve(5000);
        }

        /**
         * Save the profile result. Duration is calculated from the start and end time as the 3rd element in the vector.
         * @param _result The profile result to save.
         */
        void SaveResult(const TimeEvent _result) { ProfileResults.push_back(_result); }

        /**
         * Returns the last completed profile result.
         * @return The last profile result.
         */
        TimeEvent GetLastResult() const
        {
            if (!ProfileResults.empty()) return ProfileResults.back();
            return {};
        }

        /**
         * Erase all profile results.
         */
        void Erase() { if (!ProfileResults.empty()) ProfileResults.resize(0); }
    };

    /**
     * Profile class for profiling code.
     * Provides millisecond precision.
     */
    class Profile: public Object
    {
    public:
        Profile()
        {
            m_Timer = MakeRef<Timer>();
            m_AverageDuration = 0.0f;
            m_MinimumDuration = 0.0f;
            m_MaximumDuration = 0.0f;
        }
        Profile(const ObjectInit& _objInit);

        Ref<Timer> m_Timer;

        /**
         * Start the profiling timer.
         */
        virtual void Start();

        /**
         * End the profiling timer and record the duration.
         */
        virtual void End();

        /**
         * Get the minimum duration recorded.
         * @return Minimum duration.
         */
        float GetMinDuration() const { return m_MinimumDuration; }

        /**
         * Get the minimum duration recorded in milliseconds.
         * @return Minimum duration in milliseconds.
         */
        float GetMinDurationMs() const { return m_MinimumDuration * 1000.0f; }

        /**
         * Get the maximum duration recorded.
         * @return Maximum duration.
         */
        float GetMaxDuration() const { return m_MaximumDuration; }

        /**
         * Get the maximum duration recorded in milliseconds.
         * @return Maximum duration in milliseconds.
         */
        float GetMaxDurationMs() const { return m_MaximumDuration * 1000.0f; }

        /**
         * Get the average duration recorded.
         * @return Average duration.
         */
        float GetAverageDuration() const { return m_AverageDuration; }

        /**
         * Get the average duration recorded in milliseconds.
         * @return Average duration in milliseconds.
         */
        float GetAverageDurationMs() const { return m_AverageDuration * 1000.0f; }

        /**
         * Get the duration of the last profile.
         * @return Last profile duration.
         */
        float GetLastProfileDuration() const { return GetLastProfileResult().Duration; }

        /**
         * Get the duration of the last profile in milliseconds.
         * @return Last profile duration in milliseconds.
         */
        float GetLastPorifleDurationMs() const { return GetLastProfileResult().Duration * 1000.0f; }

        /**
         * Get the last profile result.
         * @return Last profile result.
         */
        TimeEvent GetLastProfileResult() const { return m_DurationBuffer.GetLastResult(); }

        /**
         * The average duration of the profile.
         */
        float m_AverageDuration;

        /**
         * The minimum duration recorded.
         */
        float m_MinimumDuration;

        /**
         * The maximum duration recorded.
         */
        float m_MaximumDuration;

        /**
         * Number of durations to average.
         */
        inline static int s_AverageDurationCount = 30;

        /**
         * Buffer to store profile durations.
         */
        DurationBuffer m_DurationBuffer;

        friend class ProfileSubsystem;
    };
}