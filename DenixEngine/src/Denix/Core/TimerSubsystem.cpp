#include "TimerSubsystem.h"

namespace Denix
{
	TimerSubsystem* TimerSubsystem::s_TimerSubsystem{ nullptr };

	TimerSubsystem::TimerSubsystem()
	{
		s_TimerSubsystem = this;
		DE_LOG_CREATE(LogTimer)

		m_FrameTimeS = 0.33f;
		m_FramesPerSecond = 30;
		m_GameTimeSpeed = 1.0f;
		m_MaxFPS = 60;
	}

	TimerSubsystem::~TimerSubsystem()
	{
		s_TimerSubsystem = nullptr;
	}

	void TimerSubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogTimer, Warn, "Initializing TimerSubsystem")
		DE_LOG(LogTimer, Info, "TimerSubsystem Initialized")
	}

	void TimerSubsystem::Deinitialize()
	{
		DE_LOG(LogTimer, Trace, "TimerSubsystem Deinitialized")
	}

	void TimerSubsystem::BeginFrame()
	{
		start = std::chrono::system_clock::now();
	}

	void TimerSubsystem::EndFrame()
	{
		// This really needs to be tidied up
		end = std::chrono::system_clock::now();
		std::chrono::duration<float> duration = end - start;
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

		// Enforce a maximum frame rate
		//if (frameCounter > m_MaxFPS && m_MaxFPS > 0 && timeInFrame < 1.0f) 
		//{
		//	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((1.0f - timeInFrame) * 1000.0f)));
		//}
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