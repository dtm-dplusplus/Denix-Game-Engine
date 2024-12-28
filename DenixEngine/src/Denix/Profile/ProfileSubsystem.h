#pragma once

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "Profile.h"
#include "ProfileSession.h"

#define DE_PROFILE(name) ProfileSubsystem::StartProfile(#name);
#define DE_PROFILE_END(name) ProfileSubsystem::EndProfile(#name);

#define DE_PROFILE_JOB(job) ProfileSubsystem::StartJobProfile(job);
#define DE_PROFILE_JOB_END(job) ProfileSubsystem::EndJobProfile(job);

namespace Denix
{
	struct JobDeclaration;

	class ProfileSubsystem final : public Subsystem<ProfileSubsystem>
	{
	public:
		ProfileSubsystem();

		~ProfileSubsystem() override = default;

		ProfileSubsystem(const ProfileSubsystem& _other) = delete;
		ProfileSubsystem(ProfileSubsystem&& _other) noexcept = delete;
		ProfileSubsystem& operator=(const ProfileSubsystem& _other) = delete;
		ProfileSubsystem& operator=(ProfileSubsystem&& _other) noexcept = delete;
		
		

		static void StartProfileSession();
		static void EndProfileSession();
		
		static void StartProfile(const std::string& _name);
		static void EndProfile(const std::string& _name);

		static void StartJobProfile(const Ref<JobDeclaration>& _job);
		static void EndJobProfile(const Ref<JobDeclaration>& _job);
		
		static Ref<ProfileSession> GetActiveProfileSession() { return s_Instance->m_ActiveProfileSession; }

		Ref<ProfileSession> m_ActiveProfileSession;

		std::vector<Ref<ProfileSession>> m_ProfileSessions;
		
	private:
		void Initialize() override;

		void Deinitialize() override;

		std::map<std::string, Ref<Profile>> m_Profiles;
		
		friend class Engine;
	};
}
