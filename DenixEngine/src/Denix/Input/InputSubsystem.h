#pragma once
#include "Denix/Core/Subsystem.h"
#include <SDL3/SDL_events.h>
#include "Denix/Core.h"
#include <unordered_set>
#include "InputPrimitive.h"
#include "InputHelper.h"

namespace Denix
{
	class SDL_GLWindow;
	
	class InputSubsystem: public Subsystem<InputSubsystem>
	{
	public:
		InputSubsystem() = default;
		~InputSubsystem() override = default;

		InputSubsystem(const InputSubsystem& _other) = delete;
		InputSubsystem(InputSubsystem&& _other) noexcept = delete;
		InputSubsystem& operator=(const InputSubsystem& _other) = delete;
		InputSubsystem& operator=(InputSubsystem&& _other) noexcept = delete;

		static bool IsKeyDown(KeyCode _key);
		static bool IsKeyUp(KeyCode _key);
		static bool IsMouseButtonDown(const int _button) { return s_Instance->m_MouseData.SDL_State & SDL_BUTTON(_button); }
		static MouseData& GetMouseData() { return s_Instance->m_MouseData; }

		bool m_KeyboardLogging;
		bool m_MouseLogging;
	private:
		void ProcessInputEvent(const SDL_Event& _event);
		void ProcessAudioEvent(const SDL_Event& _event);
		void ProcessPenEvent(const SDL_Event& _event);
		void ProcessCameraEvent(const SDL_Event& _event);
		
		void Initialize() override;
		void Deinitialize() override;
		void Update(float _deltaTime) override;

		//SDL_Event m_Event;
		WRef<SDL_GLWindow> m_WindowRef;

		std::unordered_set<KeyCode> m_KeysDown;
		std::unordered_set<KeyCode> m_KeysUp;
		
		// Mouse Properties
		MouseData m_MouseData;
		
		friend class Engine;
		friend class EventSubsystem;
	};
}
