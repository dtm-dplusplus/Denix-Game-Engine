#pragma once

#include <string>

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "ProfileSession.h"

// Macros for starting and ending inline profiles
#define DE_PROFILE(name) ProfileSubsystem::StartInlineProfile(#name);
#define DE_PROFILE_END(name) ProfileSubsystem::EndInlineProfile(#name);

// Macros for starting and ending job profiles
#define DE_PROFILE_JOB(job) ProfileSubsystem::StartJobProfile(job);
#define DE_PROFILE_JOB_END(job) ProfileSubsystem::EndJobProfile(job);

namespace Denix
{
    struct JobDeclaration;

    /**
     * ProfileSubsystem class for managing profiling sessions and profiles.
     * Inherits from Subsystem<ProfileSubsystem>.
     */
    class ProfileSubsystem final : public Subsystem<ProfileSubsystem>
    {
    public:
        ProfileSubsystem() = default;

        ~ProfileSubsystem() override = default;

        ProfileSubsystem(const ProfileSubsystem& _other) = delete;
        ProfileSubsystem(ProfileSubsystem&& _other) noexcept = delete;
        ProfileSubsystem& operator=(const ProfileSubsystem& _other) = delete;
        ProfileSubsystem& operator=(ProfileSubsystem&& _other) noexcept = delete;

        /**
         * Starts a new profile session with the given name.
         * @param _name Name of the profile session.
         */
        static void StartProfileSession(const std::string& _name = "ProfileSession");

        /**
         * Ends the current profile session.
         */
        static void EndProfileSession();

        /**
         * Starts an inline profile with the given name.
         * @param _name Name of the inline profile.
         */
        static void StartInlineProfile(const std::string& _name);

        /**
         * Ends the inline profile with the given name.
         * @param _name Name of the inline profile.
         */
        static void EndInlineProfile(const std::string& _name);

        /**
         * Starts a job profile for the given job.
         * @param _job Reference to the job declaration.
         */
        static void StartJobProfile(const Ref<JobDeclaration>& _job);

        /**
         * Ends the job profile for the given job.
         * @param _job Reference to the job declaration.
         */
        static void EndJobProfile(const Ref<JobDeclaration>& _job);

        /**
         * Returns the active profile session.
         * @return Reference to the active profile session.
         */
        static Ref<ProfileSession> GetActiveProfileSession() { return s_Instance->m_ActiveProfileSession; }

        /**
         * Returns the list of all profile sessions.
         * @return Vector of references to profile sessions.
         */
        static std::vector<Ref<ProfileSession>>& GetProfileSessions() { return s_Instance->m_ProfileSessions; }

    private:
        /**
         * Initializes the profile subsystem.
         */
        void Initialize() override;

        /**
         * Deinitializes the profile subsystem.
         */
        void Deinitialize() override;

        Ref<ProfileSession> m_ActiveProfileSession; ///< Active profile session.
        std::vector<Ref<ProfileSession>> m_ProfileSessions; ///< List of all profile sessions.

        friend class Engine;
    };
}
