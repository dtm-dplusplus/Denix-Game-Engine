#pragma once

#include "Profile.h"
#include "Denix/Core.h"
#include "Denix/System/Subsystem.h"
#include "Denix/Core/Timer.h"

#define DE_PROFILE(name) ProfileSubsystem::StartProfile(#name);
#define DE_PROFILE_END(name) ProfileSubsystem::EndProfile(#name);

namespace Denix
{
	class ProfileSubsystem : public Subsystem
	{
	public:
		ProfileSubsystem();

		~ProfileSubsystem() override;

		static ProfileSubsystem* Get() { return s_ProfileSubsystem; }

		void Initialize() override;

		void Deinitialize() override;

		void Update(float _deltaTime) override;
		static void StartProfile(const std::string& _name);
		static void EndProfile(const std::string& _name);

		std::map<std::string, Ref<Profile>>& GetProfiles() { return m_Profiles; }
		
	private:
		static ProfileSubsystem* s_ProfileSubsystem;

		std::map<std::string, Ref<Profile>> m_Profiles;
		
		friend class Engine;
	};
}
