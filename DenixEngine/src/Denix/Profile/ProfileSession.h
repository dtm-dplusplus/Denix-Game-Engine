#pragma once

#include "Profile.h"
#include "Denix/Thread/JobProfile.h"

#include <unordered_map>
#include <vector>

namespace Denix
{
    struct JobDeclaration;

    struct ThreadData
    {
        size_t m_ThreadID = 0;
        size_t m_JobExecCount = 0;
        float m_ThreadExecTime = 0.0f;
        float m_ThreadSleepTime = 0.0f;
    };

    class ProfileSession: public Object
    {
    public:
        ProfileSession(const ObjectInit& _objInit): Object(_objInit)
        {
            // Reserve to reduce reallocations during profiling.
            m_InlineProfileMap.reserve(50);
            m_InlineProfiles.reserve(50);
            m_JobProfilesMap.reserve(50);
            m_JobProfiles.reserve(50);
            m_SessionTimer = MakeRef<Timer>();
        }
        ~ProfileSession() override = default;

        Ref<Timer> GetSessionTimer() const { return m_SessionTimer; }

        bool IsProfiling() const { return m_IsProfiling; }
        
        std::unordered_map<std::string, Ref<Profile>>& GetInlineProfileMap() { return m_InlineProfileMap; }
        std::vector<Ref<Profile>>& GetInlineProfiles() { return m_InlineProfiles; }
        
        std::unordered_map<std::string, Ref<JobProfile>>& GetJobProfileMap() { return m_JobProfilesMap; }
        std::vector<Ref<JobProfile>>& GetJobProfiles() { return m_JobProfiles; }
        
        std::vector<ThreadData>& GetThreadData() { return m_ThreadData; }
        std::vector<int>& GetThreadJobCounts() { return m_ThreadJobCounts; }
        std::vector<float>& GetThreadJobExecTimes() { return m_ThreadJobExecTimes; }
        std::vector<float>& GetThreadSleepTimes() { return m_ThreadSleepTimes; }
        
       

       
    private:
        void StartSession();
        void EndSession();
        void StartInlineProfile(const std::string& _name);
        void EndInlineProfile(const std::string& _name);

        void StartJobProfile(const Ref<JobDeclaration>& _job);
        void EndJobProfile(const Ref<JobDeclaration>& _job);

        Ref<Profile> GetInlineProfile(const std::string& _name);
        Ref<JobProfile> GetJobProfile(const std::string& _name);

        std::unordered_map<std::string, Ref<Profile>> m_InlineProfileMap;
        std::vector<Ref<Profile>> m_InlineProfiles;
        
        std::unordered_map<std::string, Ref<JobProfile>> m_JobProfilesMap;
        std::vector<Ref<JobProfile>> m_JobProfiles;
        
        std::vector<ThreadData> m_ThreadData;
        std::vector<int> m_ThreadJobCounts;
        std::vector<float> m_ThreadJobExecTimes;
        std::vector<float> m_ThreadSleepTimes;
        
        Ref<Timer> m_SessionTimer;
        bool m_IsProfiling = false;

        friend class ProfileSubsystem;
        friend class JobSubsystem;
    };
}