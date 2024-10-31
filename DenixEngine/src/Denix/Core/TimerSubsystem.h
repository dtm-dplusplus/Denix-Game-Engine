#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Core.h"
#include "Timer.h"

namespace Denix
{
	class Profile;

	class TimerSubsystem : public Subsystem
	{
	public:
		TimerSubsystem();

		~TimerSubsystem();

		static TimerSubsystem* Get() { return s_TimerSubsystem; }

		void Initialize();

		void Deinitialize();

		void BeginFrame();

		void EndFrame();

		template<typename Period = std::chrono::seconds>
		static float GetProgramElapsedTime()
		{
			return s_TimerSubsystem->m_FrameTimer->GetElapsed<Period>();
		}
		
		static int GetFPS();
		static int& GetMaxFPS() { return s_TimerSubsystem->m_MaxFPS; }
		static float GetFrameTime();
		static float GetFrameTimeMs();
		static float& GetGameTimeSpeed() { return s_TimerSubsystem->m_GameTimeSpeed; }

		Ref<Timer> m_FrameTimer;
		Ref<Profile> m_TimerProfile;

	private:
		static TimerSubsystem* s_TimerSubsystem;

		int m_FramesPerSecond;

		/**
		 * Time taken for frame to complete in milliseconds
		 */
		float m_FrameTime;
		
		/**
		 * Time taken for frame to complete in seconds
		 */
		float m_FrameTimeS;

		/**
		 * Game specifc frame time
		 */
		float m_DeltaTime;
		
		/**
		 * 
		 */
		float m_GameTimeSpeed;

		
		/**
		 * Maximum frames per second
		 */
		int m_MaxFPS;

		
		std::vector<Ref<Timer>> m_Timers;
		
		friend class Engine;
	};
}
