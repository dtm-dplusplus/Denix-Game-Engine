#pragma once
#include "Denix/System/SubSystem.h"
#include <SDL3/SDL_events.h>
#include "Denix/Core.h"

namespace Denix
{
	struct MouseData
	{
		// SDL NewState
		Uint32 SDL_State;
		Uint32 SDL_RelativeState;

		// Movement
		float X = 0;
		float Y = 0;
		float RelX = 0;
		float RelY = 0;
		float WheelY = 0;

		// Buttons
		bool Left = false;
		bool Right = false;
		bool Middle = false;
		bool Side1 = false;
		bool Side2 = false;
	};

	class InputSubsystem: public Subsystem<InputSubsystem>
	{
	public:
		InputSubsystem();
		~InputSubsystem() override = default;

		InputSubsystem(const InputSubsystem& _other) = delete;
		InputSubsystem(InputSubsystem&& _other) noexcept = delete;
		InputSubsystem& operator=(const InputSubsystem& _other) = delete;
		InputSubsystem& operator=(InputSubsystem&& _other) noexcept = delete;

		static bool IsKeyDown(const int _key) { return s_Instance->m_SDL_KeyboardState[_key]; }
		//static bool IsKeyReleased(const int _key) { return (!s_Instance->m_SDL_KeyboardState[_key] && s_Instance->m_SDL_LastKeyboardState[_key]); }
		static bool IsMouseButtonDown(const int _button) { return s_Instance->m_MouseData.SDL_State & SDL_BUTTON(_button); }
		static MouseData& GetMouseData() { return s_Instance->m_MouseData; }

		void InputPanel();
		//SDL_Event& GetEvent() { return m_Event; }

	private:
		void Initialize() override;
		void Deinitialize() override;
		void Update(float _deltaTime) override;

		//SDL_Event m_Event;
		Ref<class Window> m_Window;

		Uint8* m_SDL_LastKeyboardState;
		const bool* m_SDL_KeyboardState;
		
		// Mouse Properties
		MouseData m_MouseData;

		void Poll();

		friend class Engine;
	};

}
