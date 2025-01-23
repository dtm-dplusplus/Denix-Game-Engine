#include "EventSubsystem.h"
#include "backends/imgui_impl_sdl3.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Video/WindowSubsystem.h"
#include "Denix/Video/GL/Viewport.h"

namespace Denix
{
    void EventSubsystem::Initialize()
    {
        DE_LOG(LogEvent, Warn, "Event Subsystem Initializing");
        Subsystem::Initialize();
        m_WindowSubsystemRef = WindowSubsystem::GetInstance();
        m_WindowRef = WindowSubsystem::GetWindow();
        m_InputRef = InputSubsystem::GetInstance();
        DE_ASSERT(m_WindowSubsystemRef.lock(), "Event: Window Subsystem reference is invalid");
        DE_ASSERT(m_WindowRef.lock(), "Event: Window reference is invalid");
        DE_ASSERT(m_InputRef.lock(), "Event: Input reference is invalid");
        
        DE_LOG(LogEvent, Info, "Event Subsystem Initialized");
    }

    void EventSubsystem::Deinitialize()
    {
        DE_LOG(LogEvent, Trace, "Event Subsystem Deinitializing");
        Subsystem::Deinitialize();
        DE_LOG(LogEvent, Trace, "Event Subsystem Deinitialized");
    }

    void EventSubsystem::Update(float _deltaTime)
    {
        DE_PROFILE(Event Update)
        Subsystem::Update(_deltaTime);

        SDL_Event event;

        Ref<InputSubsystem> input = m_InputRef.lock();
        input->m_MouseData.WheelY = 0;

        input->m_MouseData.SDL_RelativeState = SDL_GetRelativeMouseState(&input->m_MouseData.RelX, &input->m_MouseData.RelY);
        input->m_MouseData.SDL_State = SDL_GetMouseState(&input->m_MouseData.X, &input->m_MouseData.Y);

        // Check if key is still down
        /*for (const auto& key : input->m_Keyboard.keyDown | std::views::keys)
        {
            if (!input->m_Keyboard.keyStates[static_cast<SDL_Scancode>(key)]) input->m_Keyboard.keyDown.erase(key);
        }*/
       // input->m_KeysDown.clear();
        input->m_Keyboard->KeyUp.clear();

      // input->m_Keyboard.lastKeyStates = *input->m_Keyboard.keyStates;
        
        const auto window = m_WindowRef.lock();

        while (SDL_PollEvent(&event))
        {
            // ImGui Event Processing - Updates Editor Widgets
            ImGui_ImplSDL3_ProcessEvent(&event);

            // Handle SDL Input Events first (Keyboard, Mouse, Gamepad) - These are the most common
            // The following events are processed in the order of their occurence in SDL_events.h
            if (event.type >= SDL_EVENT_KEY_DOWN && event.type <= SDL_EVENT_FINGER_MOTION)
                input->ProcessInputEvent(event);

            // Handle SDL Application Events
            else if (event.type >= SDL_EVENT_QUIT && event.type <= SDL_EVENT_SYSTEM_THEME_CHANGED)
                ProcessApplicationEvent(event);

            // Handle SDL Display Events - Placholder for now
            else if (event.type >= SDL_EVENT_DISPLAY_FIRST && event.type <= SDL_EVENT_DISPLAY_LAST)
                ProcessDisplayEvent(event);

            // Handle SDL Window Events
            else if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST)
                ProcessWindowEvent(event);

            // Handle SDL File Events - Placeholder for now
            else if (event.type >= SDL_EVENT_DROP_FILE && event.type <= SDL_EVENT_DROP_TEXT)
                ProcessFileEvent(event);

            // Handle SDL Audio Events
            else if (event.type >= SDL_EVENT_AUDIO_DEVICE_ADDED && event.type <= SDL_EVENT_AUDIO_DEVICE_REMOVED)
                ProcessAudioEvent(event);

            // Handle SDL Pen Events - Placeholder for now
            else if (event.type >= SDL_EVENT_PEN_PROXIMITY_IN && event.type <= SDL_EVENT_PEN_AXIS)
                input->ProcessPenEvent(event);
            
            // Handle SDL Camera Events - Placeholder for now
            else if (event.type >= SDL_EVENT_CAMERA_DEVICE_ADDED && event.type <= SDL_EVENT_CAMERA_DEVICE_DENIED)
                input->ProcessCameraEvent(event);

            // Handle SDL Sensor Events - Placeholder for now
            else if (event.type == SDL_EVENT_SENSOR_UPDATE)
                DE_LOG(LogEvent, Trace, "Sensor Update Event")

            // Handle Unknown Events - This should never happen but here for debugging
            else DE_LOG(LogEvent, Warn, "Unknown Event Type: {}", event.type)
        }

        DE_PROFILE_END(Input Update)
    }

    void EventSubsystem::ProcessApplicationEvent(const SDL_Event& _event)
    {
        auto window = m_WindowRef.lock();

        switch (_event.type)
        {
        case SDL_EVENT_QUIT:
            {
                DE_LOG(LogInput, Trace, "Quit Event")
            }
            break;

        case SDL_EVENT_TERMINATING:
            {
                DE_LOG(LogInput, Trace, "App Terminating Event")
            }
            break;

        case SDL_EVENT_LOW_MEMORY:
            {
                DE_LOG(LogInput, Trace, "Low Memory Event")
            }
            break;

        case SDL_EVENT_WILL_ENTER_BACKGROUND:
            {
                DE_LOG(LogInput, Trace, "App Did Enter Background Event")
            }
            break;

        case SDL_EVENT_DID_ENTER_BACKGROUND:
            {
                DE_LOG(LogInput, Trace, "App Did Enter Background Event")
            }
            break;

        case SDL_EVENT_WILL_ENTER_FOREGROUND:
            {
                DE_LOG(LogInput, Trace, "App Will Enter Foreground Event")
            }
            break;

        case SDL_EVENT_DID_ENTER_FOREGROUND:
            {
                DE_LOG(LogInput, Trace, "App Did Enter Foreground Event")
            }
            break;

        case SDL_EVENT_LOCALE_CHANGED:
            {
                DE_LOG(LogInput, Trace, "Locale Changed Event")
            }
            break;

        case SDL_EVENT_SYSTEM_THEME_CHANGED:
            {
                DE_LOG(LogInput, Trace, "System Theme Changed Event")
            }
            break;

        default: DE_LOG(LogInput, Error, "Unknown Application Event")
        }
    }

    void EventSubsystem::ProcessWindowEvent(const SDL_Event& _event)
    {
        const auto window = m_WindowRef.lock();
        DE_ASSERT(window, "Event: Window reference is invalid");

        switch (_event.type)
        {
        case SDL_EVENT_WINDOW_SHOWN: break; /**< Window has been shown */
        case SDL_EVENT_WINDOW_HIDDEN: break; /**< Window has been hidden */
        case SDL_EVENT_WINDOW_EXPOSED:
            {
                /**< Window has been exposed and should be redrawn:
                and can be redrawn directly from event watchers for this event */
            }
            break;
        case SDL_EVENT_WINDOW_MOVED: break; /**< Window has been moved to data1: data2 */
        case SDL_EVENT_WINDOW_RESIZED:
            {
                // Check the window reference
                if (_event.window.windowID != SDL_GetWindowID(window->GetSDLWindow())) break;

                /**< Window has been resized to data1xdata2 */
                window->m_WinX = _event.window.data1;
                window->m_WinY = _event.window.data2;
                glViewport(0, 0, window->m_WinX, window->m_WinY);

                // The window should have a shared pointer to the active viewport so we don't need to do this
                SceneSubsystem::GetActiveCamera()->GetCameraComponent()->GetViewport()->m_FrameBuffer->Resize(
                    window->m_WinX, window->m_WinY);

                DE_LOG(LogWindow, Trace, "Window Resized Event. Res: {}x{}", window->m_WinX, window->m_WinY)
            }
            break; /**< Window has been resized to data1xdata2 */
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: break; /**< The pixel size of the window has changed to data1xdata2 */
        case SDL_EVENT_WINDOW_MINIMIZED:
            {
                DE_LOG(LogWindow, Trace, "Window Minimized Event")
            }
            break; /**< Window has been minimized */
        case SDL_EVENT_WINDOW_MAXIMIZED:
            {
                DE_LOG(LogWindow, Trace, "Window Maximized Event")
            }
            break; /**< Window has been maximized */
        case SDL_EVENT_WINDOW_RESTORED: break; /**< Window has been restored to normal size and position */
        case SDL_EVENT_WINDOW_MOUSE_ENTER: break; /**< Window has gained mouse focus */
        case SDL_EVENT_WINDOW_MOUSE_LEAVE: break; /**< Window has lost mouse focus */
        case SDL_EVENT_WINDOW_FOCUS_GAINED: break; /**< Window has gained keyboard focus */
        case SDL_EVENT_WINDOW_FOCUS_LOST: break; /**< Window has lost keyboard focus */
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            {
                window->RequestClose();
                DE_LOG(LogWindow, Trace, "Window Close Event")
            }
            break; /**< The window manager requests that the window be closed */
        case SDL_EVENT_WINDOW_HIT_TEST: break; /**< Window had a hit test that wasn't SDL_HITTEST_NORMAL */
        case SDL_EVENT_WINDOW_ICCPROF_CHANGED: break; /**< The ICC profile of the window's display has changed */
        case SDL_EVENT_WINDOW_DISPLAY_CHANGED: break; /**< Window has been moved to display data1 */
        case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED: break; /**< Window display scale has been changed */
        case SDL_EVENT_WINDOW_OCCLUDED: break; /**< The window has been occluded */
        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN: break; /**< The window has entered fullscreen mode */
        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN: break; /**< The window has left fullscreen mode */
        case SDL_EVENT_WINDOW_DESTROYED:
            {
                /**< The window with the associated ID is being or has been destroyed. If this message is being handled
                in an event watcher, the window handle is still valid and can still be used to retrieve any userdata
                associated with the window. Otherwise, the handle has already been destroyed and all resources
                associated with it are invalid */

                //m_Window->WindowEvent(&event);
            }
            break;
        }
    }

    void EventSubsystem::ProcessDisplayEvent(const SDL_Event& _event)
    {
        switch (_event.type)
        {
        case SDL_EVENT_DISPLAY_ORIENTATION: break; /**< Display orientation has changed to data1 */
        case SDL_EVENT_DISPLAY_ADDED: break; /**< Display has been added to the system */
        case SDL_EVENT_DISPLAY_REMOVED: break; /**< Display has been removed from the system */
        case SDL_EVENT_DISPLAY_MOVED: break; /**< Display has changed position */
        case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED: break; /**< Display has changed desktop mode */
        case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED: break; /**< Display has changed current mode */
        case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED: break; /**< Display has changed content scale */
        default: DE_LOG(LogInput, Error, "Unknown Display Event")
        }
    }

    void EventSubsystem::ProcessFileEvent(const SDL_Event& _event)
    {
        switch (_event.type)
        {
        /* Clipboard events */
        case SDL_EVENT_CLIPBOARD_UPDATE:
            DE_LOG(LogInput, Trace, "Clipboard Update Event");
            break;

        /* Drag and drop events */
        case SDL_EVENT_DROP_FILE:
            DE_LOG(LogInput, Trace, "Drop File Event");
            break;
        case SDL_EVENT_DROP_TEXT:
            DE_LOG(LogInput, Trace, "Drop Text Event");
            break;
        case SDL_EVENT_DROP_BEGIN:
            DE_LOG(LogInput, Trace, "Drop Begin Event");
            break;
        case SDL_EVENT_DROP_COMPLETE:
            DE_LOG(LogInput, Trace, "Drop Complete Event");
            break;
        case SDL_EVENT_DROP_POSITION:
            DE_LOG(LogInput, Trace, "Drop Position Event");
            break;

        default:
            DE_LOG(LogInput, Error, "Unknown File Event");
        }
    }

    void EventSubsystem::ProcessAudioEvent(const SDL_Event& _event)
    {
        switch (_event.type)
        {
        case SDL_EVENT_AUDIO_DEVICE_ADDED:
            DE_LOG(LogInput, Trace, "Audio Device Added Event");
            break;
        case SDL_EVENT_AUDIO_DEVICE_REMOVED:
            DE_LOG(LogInput, Trace, "Audio Device Removed Event");
            break;
        case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
            DE_LOG(LogInput, Trace, "Audio Device Format Changed Event");
            break;
        default:
            DE_LOG(LogInput, Error, "Unknown Audio Event");
        }
    }

    void EventSubsystem::ProcessRenderEvent(const SDL_Event& _event)
    {
        switch (_event.type)
        {
            case SDL_EVENT_RENDER_TARGETS_RESET:
                DE_LOG(LogInput, Trace, "Render Targets Reset Event");
                break;
            case SDL_EVENT_RENDER_DEVICE_RESET:
                DE_LOG(LogInput, Trace, "Render Device Reset Event");
                break;
            default:
                DE_LOG(LogInput, Error, "Unknown Render Event");
        }
    }
}