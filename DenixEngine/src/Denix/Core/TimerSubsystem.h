#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Core.h"
#include "Timer.h"

namespace Denix
{
	class Profile;

	class TimerSubsystem final : public Subsystem<TimerSubsystem>
	{
	public:
		TimerSubsystem();
		~TimerSubsystem() override = default;

		TimerSubsystem(const TimerSubsystem& _other) = delete;
		TimerSubsystem(TimerSubsystem&& _other) noexcept = delete;
		TimerSubsystem& operator=(const TimerSubsystem& _other) = delete;
		TimerSubsystem& operator=(TimerSubsystem&& _other) noexcept = delete;
		
		void Initialize() override;

		void Deinitialize() override;

		void BeginFrame();
		void EndFrame();

		static float GetProgramElaspedTime();
		static int GetFPS();
		static int& GetMaxFPS() { return s_Instance->m_MaxLimitFPS; }
		static void SetMaxFPS(int _maxFPS) { s_Instance->m_MaxLimitFPS = _maxFPS; }
		static float GetFrameTime();
		static float GetFrameTimeMs();
		static float GetAverageFrameTime();
		static float GetAverageFrameTimeMs();
		
		static float GetDeltaTime() {return s_Instance->m_DeltaTime; }
		static float& GetGameTimeSpeed() { return s_Instance->m_GameTimeSpeed; }

		Ref<Profile> m_EngineProfile;

	private:
		int m_FramesPerSecond;

		/**
		 * Time taken for frame to complete in milliseconds
		 */
		float m_FrameTime;
		
		/**
		 * Game specifc frame time
		 */
		float m_DeltaTime;
		
		/**
		 * 
		 */
		float m_GameTimeSpeed;

		
		/**
		 * 
		 */
		int m_MaxLimitFPS;

		
		std::vector<Ref<Timer>> m_Timers;
		
		friend class Engine;
		friend class ProfileSubsystem;
	};
}
