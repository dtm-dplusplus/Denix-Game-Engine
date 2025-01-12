#pragma once

#include <string>

#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "ProfileSession.h"

#define DE_PROFILE(name) ProfileSubsystem::StartInlineProfile(#name);
#define DE_PROFILE_END(name) ProfileSubsystem::EndInlineProfile(#name);

#define DE_PROFILE_JOB(job) ProfileSubsystem::StartJobProfile(job);
#define DE_PROFILE_JOB_END(job) ProfileSubsystem::EndJobProfile(job);

namespace Denix
{
	struct JobDeclaration;

	class ProfileSubsystem final : public Subsystem<ProfileSubsystem>
	{
	public:
		ProfileSubsystem() = default;

		~ProfileSubsystem() override = default;

		ProfileSubsystem(const ProfileSubsystem& _other) = delete;
		ProfileSubsystem(ProfileSubsystem&& _other) noexcept = delete;
		ProfileSubsystem& operator=(const ProfileSubsystem& _other) = delete;
		ProfileSubsystem& operator=(ProfileSubsystem&& _other) noexcept = delete;

		static void StartProfileSession(const std::string& _name = "ProfileSession");
		static void EndProfileSession();
		
		static void StartInlineProfile(const std::string& _name);
		static void EndInlineProfile(const std::string& _name);

		static void StartJobProfile(const Ref<JobDeclaration>& _job);
		static void EndJobProfile(const Ref<JobDeclaration>& _job);
		
		static Ref<ProfileSession> GetActiveProfileSession() { return s_Instance->m_ActiveProfileSession; }

		static std::vector<Ref<ProfileSession>>& GetProfileSessions() { return s_Instance->m_ProfileSessions; }
		
	private:
		void Initialize() override;

		void Deinitialize() override;

		Ref<ProfileSession> m_ActiveProfileSession;

		std::vector<Ref<ProfileSession>> m_ProfileSessions;
		
		friend class Engine;
	};
}
