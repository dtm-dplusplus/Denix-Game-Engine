#include "InputSubsystem.h"

#include "backends/imgui_impl_sdl3.h"
#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Video/Window/WindowSubsystem.h"

namespace Denix
{
	InputSubsystem* InputSubsystem::s_InputSubsystem{ nullptr };

	void InputSubsystem::Initialize()
	{
		Subsystem::Initialize();
		DE_LOG(LogInput, Warn, "Input Subsystem Initializing");

		m_Window = WindowSubsystem::Get()->GetWindow();
      	m_SDL_KeyboardState = SDL_GetKeyboardState(NULL);

		DE_LOG(LogInput, Info, "Input Subsystem Initialized");
	}

	void InputSubsystem::Deinitialize()
	{
		m_Window = nullptr;
        m_SDL_KeyboardState = nullptr;
		Subsystem::Deinitialize();
	}

	void InputSubsystem::Update(float _deltaTime)
	{
		Subsystem::Update(_deltaTime);

		Poll();
	}

	void InputSubsystem::Poll()
	{
		DE_PROFILE(Input Poll)
		SDL_Event event;

        m_MouseData.WheelY = 0;

		m_MouseData.SDL_RelativeState = SDL_GetRelativeMouseState(&m_MouseData.RelX, &m_MouseData.RelY);
		m_MouseData.SDL_State = SDL_GetMouseState(&m_MouseData.X, &m_MouseData.Y);
		m_SDL_KeyboardState = SDL_GetKeyboardState(NULL);
		
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);

            switch (event.type)
            {
            // All window events will be passed to window subsystem for handling

            case SDL_WINDOW_MINIMIZED: {}
            case SDL_WINDOW_MAXIMIZED:{}
			case SDL_EVENT_WINDOW_SHOWN:    { /**< Window has been shown */}	
		    case SDL_EVENT_WINDOW_HIDDEN:           { /**< Window has been hidden */} 
		    case SDL_EVENT_WINDOW_EXPOSED:
			    {
				    /*< Window has been exposed and should be redrawn:
					and can be redrawn directly from event watchers for this event */
			    } 
		    case SDL_EVENT_WINDOW_MOVED:           {  /**< Window has been moved to data1: data2 */} 
		    case SDL_EVENT_WINDOW_RESIZED:          { /**< Window has been resized to data1xdata2 */}	
		    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:{/**< The pixel size of the window has changed to data1xdata2 */}	
		    case SDL_EVENT_WINDOW_MINIMIZED:         {/**< Window has been minimized */}	
		    case SDL_EVENT_WINDOW_MAXIMIZED:         {/**< Window has been maximized */}	
		    case SDL_EVENT_WINDOW_RESTORED:          {/**< Window has been restored to normal size and position */}	
		    case SDL_EVENT_WINDOW_MOUSE_ENTER:       {/**< Window has gained mouse focus */}	
		    case SDL_EVENT_WINDOW_MOUSE_LEAVE:       {/**< Window has lost mouse focus */}	
		    case SDL_EVENT_WINDOW_FOCUS_GAINED:      {/**< Window has gained keyboard focus */}	
		    case SDL_EVENT_WINDOW_FOCUS_LOST:        {/**< Window has lost keyboard focus */}	
		    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:   {/**< The window manager requests that the window be closed */}	
		    case SDL_EVENT_WINDOW_HIT_TEST:          {/**< Window had a hit test that wasn't SDL_HITTEST_NORMAL */}	
		    case SDL_EVENT_WINDOW_ICCPROF_CHANGED:   {/**< The ICC profile of the window's display has changed */}	
		    case SDL_EVENT_WINDOW_DISPLAY_CHANGED:   {/**< Window has been moved to display data1 */}	
            case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED: {/**< Window display scale has been changed */}	
		    case SDL_EVENT_WINDOW_OCCLUDED:          {/**< The window has been occluded */}		
		    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:  {/**< The window has entered fullscreen mode */}	
		    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:  {/**< The window has left fullscreen mode */}	
		    case SDL_EVENT_WINDOW_DESTROYED:
			    {
				    /**< The window with the associated ID is being or has been destroyed. If this message is being handled
					  in an event watcher, the window handle is still valid and can still be used to retrieve any userdata
					  associated with the window. Otherwise, the handle has already been destroyed and all resources
					  associated with it are invalid */

					WindowSubsystem::GetWindow()->WindowEvent(&event);
			    }	break;

            
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

		DE_PROFILE_END(Input Poll)
	}

	void InputSubsystem::InputPanel()
	{
        
	}
}