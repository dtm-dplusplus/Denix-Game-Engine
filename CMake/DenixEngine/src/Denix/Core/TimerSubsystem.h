#pragma once

#include "Denix/System/Subsystem.h"
#include "Denix/Core/Logger.h"
#include <chrono>

namespace Denix
{
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

		static int GetFPS();
		static float GetFrameTime();
		static float GetFrameTimeMs();
		static float& GetGameTimeSpeed() { return s_TimerSubsystem->m_GameTimeSpeed; }
	private:
		static TimerSubsystem* s_TimerSubsystem;

		std::chrono::time_point<std::chrono::system_clock> start, end;

		int m_FramesPerSecond;
		float m_FrameTimeS;
		float m_FrameTimeMs;
		float m_DeltaTime;
		float m_GameTimeSpeed;


		friend class Engine;
	};
}
