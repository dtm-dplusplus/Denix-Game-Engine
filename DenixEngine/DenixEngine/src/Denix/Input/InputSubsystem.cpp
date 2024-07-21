#include "depch.h"
#include "InputSubsystem.h"

#include "backends/imgui_impl_sdl3.h"
#include "Denix/Video/Window/WindowSubsystem.h"

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
			ImGui_ImplSDL3_ProcessEvent(&event);

            switch (event.type)
            {
	            /*case SDL_EVENT_WINDOW_CLOSE_REQUESTED: // Additioanl CHeck e.window.windowID == SDL_GetWindowID(m_SDL_GLWindow)
            		{
            			m_IsOpen = false;
            			DE_LOG(LogWindow, Trace, "Window Close Event")
					}
            		break;

            	case SDL_WINDOW_MINIMIZED:
            		{
            			DE_LOG(LogWindow, Trace, "Window Minimized Event")
					}
            		break;

            	case SDL_WINDOW_MAXIMIZED:
            		{
            			DE_LOG(LogWindow, Trace, "Window Maximized Event")
					}
            		break;

            	case SDL_EVENT_WINDOW_RESIZED:
            		{
            			m_WinX = _event->window.data1;
            			m_WinY = _event->window.data2;
            			glViewport(0, 0, m_WinX, m_WinY);

            			DE_LOG(LogWindow, Trace, "Window Resized Event. Res: {}x{}", m_WinX, m_WinY)
					}
            		break;

            	default:	break;
            	}*/

            case SDL_EVENT_MOUSE_WHEEL:
            {
                m_MouseData.WheelY = event.wheel.y;
            } break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                if(event.button.button == SDL_BUTTON_LEFT) m_MouseData.Left = true;
                if(event.button.button == SDL_BUTTON_RIGHT) m_MouseData.Right = true;
                if(event.button.button == SDL_BUTTON_MIDDLE) m_MouseData.Middle = true;
                if(event.button.button == SDL_BUTTON_X1) m_MouseData.Side1 = true;
                if(event.button.button == SDL_BUTTON_X2) m_MouseData.Side2 = true;
            } break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                if (event.button.button == SDL_BUTTON_LEFT)  m_MouseData.Left = false;
                if (event.button.button == SDL_BUTTON_RIGHT) m_MouseData.Right = false;
                if (event.button.button == SDL_BUTTON_MIDDLE) m_MouseData.Middle = false;
                if (event.button.button == SDL_BUTTON_X1) m_MouseData.Side1 = false;
                if (event.button.button == SDL_BUTTON_X2) m_MouseData.Side2 = false;

            } break;
           
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
            {
               // DE_LOG(LogInput, Trace, "Key: {}", event.key.keysym.scancode);
            } break;

            default: ;
            }
		}


        /*m_MouseData.SDL_RelativeState = SDL_GetRelativeMouseState(&m_MouseData.RelX, &m_MouseData.RelY);
        m_MouseData.SDL_State = SDL_GetMouseState(&m_MouseData.X, &m_MouseData.Y);*/

        m_SDL_KeyboardState = SDL_GetKeyboardState(NULL);
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