#pragma once
#include "Denix/System/SubSystem.h"
#include "SDL_events.h"
#include "Denix/Core.h"

namespace Denix
{
	struct MouseData
	{
		// SDL State
		Uint32 SDL_State;
		Uint32 SDL_RelativeState;

		// Movement
		int X = 0;
		int Y = 0;
		int RelX = 0;
		int RelY = 0;
		int WheelY = 0;

		// Buttons
		bool Left = false;
		bool Right = false;
		bool Middle = false;
		bool Side1 = false;
		bool Side2 = false;
	};

	class InputSubsystem: public Subsystem
	{
	public:
		InputSubsystem()
		{
			s_InputSubsystem = this;
		}
		~InputSubsystem() override
		{
			s_InputSubsystem = nullptr;
		}

		bool IsKeyDown(const int _key) const { return m_SDL_KeyboardState[_key]; }
		bool IsMouseButtonDown(const int _button) const { return m_MouseData.SDL_State & SDL_BUTTON(_button); }
		MouseData& GetMouseData() { return m_MouseData; }

		static InputSubsystem* Get() { return s_InputSubsystem; }

		void Initialize() override;
		void Deinitialize() override;

		void Poll();

		void InputPanel();
		//SDL_Event& GetEvent() { return m_Event; }

	private:
		static InputSubsystem* s_InputSubsystem;

		//SDL_Event m_Event;
		Ref<class Window> m_Window;

		const Uint8* m_SDL_KeyboardState;
		

		// Mouse Properties
		MouseData m_MouseData;
	};

}
