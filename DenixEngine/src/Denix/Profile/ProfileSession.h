#pragma once
#include "Profile.h"
#include "Denix/Thread/JobProfile.h"

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
            m_InlineProfiles.reserve(25);
        }
        ~ProfileSession() override = default;

        std::unordered_map<std::string, Profile>& GetInlineProfiles() { return m_InlineProfiles; }
        std::unordered_map<std::string, JobProfile>& GetJobProfiles() { return m_JobProfiles; }
        std::vector<ThreadData>& GetThreadData() { return m_ThreadData; }
        
        bool IsProfiling() const { return m_IsProfiling; }
        
    private:
        void StartInlineProfile(const std::string& _name);
        void EndInlineProfile(const std::string& _name);

        void StartJobProfile(const Ref<JobDeclaration>& _job);
        void EndJobProfile(const Ref<JobDeclaration>& _job);
        
        Profile& GetInlineProfile(const std::string& _name);
        JobProfile& GetJobProfile(const std::string& _name);
        
        std::unordered_map<std::string, Profile> m_InlineProfiles;
        
        std::unordered_map<std::string, JobProfile> m_JobProfiles;

        std::vector<ThreadData> m_ThreadData;
        
        bool m_IsProfiling = false;

        friend class ProfileSubsystem;
        friend class JobSubsystem;
    };


}
