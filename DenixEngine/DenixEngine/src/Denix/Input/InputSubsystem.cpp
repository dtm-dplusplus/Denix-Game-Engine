#include "depch.h"
#include "InputSubsystem.h"

#include "backends/imgui_impl_sdl2.h"
#include "Denix/System/WindowSubsystem.h"

namespace Denix
{
	InputSubsystem* InputSubsystem::s_InputSubsystem{ nullptr };

	void InputSubsystem::Initialize()
	{
		Subsystem::Initialize();

        DE_LOG_CREATE(LogInput)

		m_Window = WindowSubsystem::Get()->GetWindow();
        m_SDL_KeyboardState = SDL_GetKeyboardState(NULL);
	}

	void InputSubsystem::Deinitialize()
	{
		m_Window = nullptr;
        m_SDL_KeyboardState = nullptr;
		Subsystem::Deinitialize();
	}

	void InputSubsystem::Poll()
	{
		//m_Event = SDL_Event();
		SDL_Event event;

        m_MouseData.WheelY = 0;

		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type)
            {
            case SDL_WINDOWEVENT:
            {
                m_Window->WindowEvent(&event);
            } break;

            case SDL_MOUSEWHEEL:
            {
                m_MouseData.WheelY = event.wheel.y;
            } break;

            case SDL_MOUSEBUTTONDOWN:
            {
                if(event.button.button == SDL_BUTTON_LEFT) m_MouseData.Left = true;
                if(event.button.button == SDL_BUTTON_RIGHT) m_MouseData.Right = true;
                if(event.button.button == SDL_BUTTON_MIDDLE) m_MouseData.Middle = true;
                if(event.button.button == SDL_BUTTON_X1) m_MouseData.Side1 = true;
                if(event.button.button == SDL_BUTTON_X2) m_MouseData.Side2 = true;
            } break;
            case SDL_MOUSEBUTTONUP:
            {
                if (event.button.button == SDL_BUTTON_LEFT)  m_MouseData.Left = false;
                if (event.button.button == SDL_BUTTON_RIGHT) m_MouseData.Right = false;
                if (event.button.button == SDL_BUTTON_MIDDLE) m_MouseData.Middle = false;
                if (event.button.button == SDL_BUTTON_X1) m_MouseData.Side1 = false;
                if (event.button.button == SDL_BUTTON_X2) m_MouseData.Side2 = false;

            } break;
           
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
               // DE_LOG(LogInput, Trace, "Key: {}", event.key.keysym.scancode);
            } break;

            default: ;
            }
		}


        m_MouseData.SDL_RelativeState = SDL_GetRelativeMouseState(&m_MouseData.RelX, &m_MouseData.RelY);
        m_MouseData.SDL_State = SDL_GetMouseState(&m_MouseData.X, &m_MouseData.Y);
        
	}

	void InputSubsystem::InputPanel()
	{
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_Appearing);
		ImGui::Begin("Input");
        ImGui::SeparatorText("Mouse Motion");
        ImGui::Text("Mouse Pos: %d %d", m_MouseData.X, m_MouseData.Y);
        ImGui::Text("Mouse Rel: %d %d", m_MouseData.RelX, m_MouseData.RelY);
        ImGui::Text("Mouse Wheel: %d", m_MouseData.WheelY);

        ImGui::SeparatorText("Mouse Buttons");
        ImGui::Text("Left: %s", m_MouseData.Left ? "True" : "False");
        ImGui::Text("Right: %s", m_MouseData.Right ? "True" : "False");
        ImGui::Text("Middle: %s", m_MouseData.Middle ? "True" : "False");
        ImGui::Text("Side1: %s", m_MouseData.Side1 ? "True" : "False");
        ImGui::Text("Side2: %s", m_MouseData.Side2 ? "True" : "False");

		ImGui::End();
	}
}