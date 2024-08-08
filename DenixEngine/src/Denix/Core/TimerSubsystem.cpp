#include "TimerSubsystem.h"

namespace Denix
{
	TimerSubsystem* TimerSubsystem::s_TimerSubsystem{ nullptr };

	TimerSubsystem::TimerSubsystem() : m_FrameTimeS{ 0.33f }, m_FramesPerSecond{ 30 }, m_GameTimeSpeed{ 1.0f }
	{
		DE_LOG_CREATE(LogTimer)
			s_TimerSubsystem = this;
	}

	TimerSubsystem::~TimerSubsystem()
	{
		s_TimerSubsystem = nullptr;
	}

	void TimerSubsystem::Initialize()
	{
		DE_LOG(LogTimer, Trace, "TimerSubsystem Initialized")

			m_Initialized = true;
	}

	void TimerSubsystem::Deinitialize()
	{
		DE_LOG(LogTimer, Trace, "TimerSubsystem Deinitialized")
			m_Initialized = false;
	}

	void TimerSubsystem::BeginFrame()
	{
		start = std::chrono::system_clock::now();
	}

	void TimerSubsystem::EndFrame()
	{
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> duration = end - start;
		m_FrameTimeS = duration.count();
		m_FrameTimeMs = m_FrameTimeS * 1000.0f;
		m_DeltaTime = m_GameTimeSpeed * m_FrameTimeS;

		static int frameCounter = 0;
		frameCounter++;

		static float timeInFrame = 0.0f;
		timeInFrame += m_FrameTimeS;

		if (timeInFrame >= 1.0f)
		{
			m_FramesPerSecond = frameCounter;
			frameCounter = 0;
			timeInFrame = 0.0f;
		}
	}

	int TimerSubsystem::GetFPS()
	{
		return s_TimerSubsystem->m_FramesPerSecond;
	}

	float TimerSubsystem::GetFrameTime() { return s_TimerSubsystem->m_FrameTimeS; }
	float TimerSubsystem::GetFrameTimeMs()
	{
		return s_TimerSubsystem->m_FrameTimeMs;
	}
}