#pragma once

#include "Denix/Core.h"
#include "Denix/Core/Subsystem.h"

#include "Denix/Video/Window/Window.h"
#include "GL/Viewport.h"

namespace Denix
{
	class WindowSubsystem: public Subsystem<WindowSubsystem>
	{
	public:
		WindowSubsystem() = default;

		~WindowSubsystem() override = default;

		static void ToggleFullscreen();

		static Ref<SDL_GLWindow> GetWindow() { return s_Instance->m_Window; }
		
		static glm::vec2 GetWindowSize() { return s_Instance->m_Window->GetWindowSize(); }
		static int GetWindowWidth() { return s_Instance->m_Window->GetWidth(); }
		static int GetWindowHeight() { return s_Instance->m_Window->GetHeight(); }
		
	private:
		void NewFrame();
		void PresentFrame();
		void Initialize() override;

		void Deinitialize() override;

		Ref<SDL_GLWindow> m_Window;

		friend class Engine;
	};
}
