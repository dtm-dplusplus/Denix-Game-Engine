#include "InputSubsystem.h"

#include "backends/imgui_impl_sdl3.h"
#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Video/WindowSubsystem.h"
#include "Denix/Video/GL/Viewport.h"

namespace Denix
{
    void InputSubsystem::Initialize()
    {
        Subsystem::Initialize();
        DE_LOG(LogInput, Warn, "Input Subsystem Initializing");
        m_WindowRef = WindowSubsystem::GetWindow();
        m_KeyboardLogging = false;
        m_MouseLogging = false;
        DE_LOG(LogInput, Info, "Input Subsystem Initialized");
    }

    void InputSubsystem::Deinitialize()
    {
        DE_LOG(LogInput, Trace, "Input Subsystem Deinitializing");
        Subsystem::Deinitialize();
        DE_LOG(LogInput, Trace, "Input Subsystem Deinitialized");
    }

    void InputSubsystem::Update(float _deltaTime)
    {
        DE_PROFILE(Input Update)
        Subsystem::Update(_deltaTime);

        SDL_Event event;

        m_MouseData.WheelY = 0;

        m_MouseData.SDL_RelativeState = SDL_GetRelativeMouseState(&m_MouseData.RelX, &m_MouseData.RelY);
        m_MouseData.SDL_State = SDL_GetMouseState(&m_MouseData.X, &m_MouseData.Y);

        m_KeysDown.clear();
        m_KeysUp.clear();

        const auto window = m_WindowRef.lock();

        while (SDL_PollEvent(&event))
        {
            // ImGui Event Processing - Updates Editor Widgets
            ImGui_ImplSDL3_ProcessEvent(&event);

            // Handle SDL Input Events first (Keyboard, Mouse, Gamepad) - These are the most common
            // The following events are processed in the order of their occurence in SDL_events.h
            if (event.type >= SDL_EVENT_KEY_DOWN && event.type <= SDL_EVENT_FINGER_MOTION)
                ProcessInputEvent(event);

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
                ProcessPenEvent(event);
            
            // Handle SDL Camera Events - Placeholder for now
            else if (event.type >= SDL_EVENT_CAMERA_DEVICE_ADDED && event.type <= SDL_EVENT_CAMERA_DEVICE_DENIED)
                ProcessCameraEvent(event);
        }

        DE_PROFILE_END(Input Update)
    }

    bool InputSubsystem::IsKeyDown(const KeyCode _key)
    {
        return s_Instance->m_KeysDown.contains(_key);
    }

    bool InputSubsystem::IsKeyUp(const KeyCode _key)
    {
        return s_Instance->m_KeysUp.contains(_key);
    }

    void InputSubsystem::ProcessApplicationEvent(const SDL_Event& _event)
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

    void InputSubsystem::ProcessWindowEvent(const SDL_Event& _event)
    {
        const auto window = m_WindowRef.lock();
        DE_ASSERT(window, "Input: Window reference is invalid");

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

    void InputSubsystem::ProcessDisplayEvent(const SDL_Event& _event)
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

    void InputSubsystem::ProcessInputEvent(const SDL_Event& _event)
    {
        switch (_event.type)
        {
        /* Keyboard events */
        case SDL_EVENT_KEY_DOWN:
            {
                m_KeysDown.insert(static_cast<KeyCode>(_event.key.key));
            }
            break; /**< Key pressed */
        case SDL_EVENT_KEY_UP:
            {
                m_KeysUp.insert(static_cast<KeyCode>(_event.key.key));
                if (m_KeyboardLogging)
                    DE_LOG(LogInput, Trace, "Key Up Event. Key: {}", SDL_GetKeyName(_event.key.key))
            }
            break; /**< Key released */
        case SDL_EVENT_TEXT_EDITING: break; /**< Keyboard text editing (composition) */
        case SDL_EVENT_TEXT_INPUT: break; /**< Keyboard text input */
        case SDL_EVENT_KEYMAP_CHANGED: break;
        /**< Keymap changed due to a system event such as an input language or keyboard layout change. */
        case SDL_EVENT_KEYBOARD_ADDED: break; /**< A new keyboard has been inserted into the system */
        case SDL_EVENT_KEYBOARD_REMOVED: break; /**< A keyboard has been removed */
        case SDL_EVENT_TEXT_EDITING_CANDIDATES: break; /**< Keyboard text editing candidates */

        /* Mouse events */
        case SDL_EVENT_MOUSE_MOTION: break; /**< Mouse moved */
        case SDL_EVENT_MOUSE_BUTTON_DOWN: break; /**< Mouse button pressed */
        case SDL_EVENT_MOUSE_BUTTON_UP: break; /**< Mouse button released */
        case SDL_EVENT_MOUSE_WHEEL:
            {
                m_MouseData.WheelY = _event.wheel.y;
            }break; /**< Mouse wheel motion */
        case SDL_EVENT_MOUSE_ADDED: break; /**< A new mouse has been inserted into the system */
        case SDL_EVENT_MOUSE_REMOVED: break; /**< A mouse has been removed */

        /* Joystick events */
        case SDL_EVENT_JOYSTICK_AXIS_MOTION: break; /**< Joystick axis motion */
        case SDL_EVENT_JOYSTICK_BALL_MOTION: break; /**< Joystick trackball motion */
        case SDL_EVENT_JOYSTICK_HAT_MOTION: break; /**< Joystick hat position change */
        case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
            {
                if (_event.button.button == SDL_BUTTON_LEFT) m_MouseData.Left = true;
                if (_event.button.button == SDL_BUTTON_RIGHT) m_MouseData.Right = true;
                if (_event.button.button == SDL_BUTTON_MIDDLE) m_MouseData.Middle = true;
                if (_event.button.button == SDL_BUTTON_X1) m_MouseData.Side1 = true;
                if (_event.button.button == SDL_BUTTON_X2) m_MouseData.Side2 = true;
            }break; /**< Joystick button pressed */
        case SDL_EVENT_JOYSTICK_BUTTON_UP:
            {
                if (_event.button.button == SDL_BUTTON_LEFT) m_MouseData.Left = false;
                if (_event.button.button == SDL_BUTTON_RIGHT) m_MouseData.Right = false;
                if (_event.button.button == SDL_BUTTON_MIDDLE) m_MouseData.Middle = false;
                if (_event.button.button == SDL_BUTTON_X1) m_MouseData.Side1 = false;
                if (_event.button.button == SDL_BUTTON_X2) m_MouseData.Side2 = false;
            }break; /**< Joystick button released */
        case SDL_EVENT_JOYSTICK_ADDED: break; /**< A new joystick has been inserted into the system */
        case SDL_EVENT_JOYSTICK_REMOVED: break; /**< An opened joystick has been removed */
        case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: break; /**< Joystick battery level change */
        case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: break; /**< Joystick update is complete */

        /* Gamepad events */
        case SDL_EVENT_GAMEPAD_AXIS_MOTION: break; /**< Gamepad axis motion */
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN: break; /**< Gamepad button pressed */
        case SDL_EVENT_GAMEPAD_BUTTON_UP: break; /**< Gamepad button released */
        case SDL_EVENT_GAMEPAD_ADDED: break; /**< A new gamepad has been inserted into the system */
        case SDL_EVENT_GAMEPAD_REMOVED: break; /**< A gamepad has been removed */
        case SDL_EVENT_GAMEPAD_REMAPPED: break; /**< The gamepad mapping was updated */
        case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN: break; /**< Gamepad touchpad was touched */
        case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: break; /**< Gamepad touchpad finger was moved */
        case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: break; /**< Gamepad touchpad finger was lifted */
        case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: break; /**< Gamepad sensor was updated */
        case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: break; /**< Gamepad update is complete */
        case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED: break; /**< Gamepad Steam handle has changed */

        /* Touch events */
        case SDL_EVENT_FINGER_DOWN: break;
        case SDL_EVENT_FINGER_UP: break;
        case SDL_EVENT_FINGER_MOTION: break;
        default: DE_LOG(LogInput, Error, "Unknown Input Event")
        }
    }

    void InputSubsystem::ProcessFileEvent(const SDL_Event& _event)
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

    void InputSubsystem::ProcessAudioEvent(const SDL_Event& _event)
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

    void InputSubsystem::ProcessPenEvent(const SDL_Event& _event)
    {
        switch (_event.type)
        {
            case SDL_EVENT_PEN_PROXIMITY_IN:
                DE_LOG(LogInput, Trace, "Pen Proximity In Event");
                break;
            case SDL_EVENT_PEN_PROXIMITY_OUT:
                DE_LOG(LogInput, Trace, "Pen Proximity Out Event");
                break;
            case SDL_EVENT_PEN_DOWN:
                DE_LOG(LogInput, Trace, "Pen Down Event");
                break;
            case SDL_EVENT_PEN_UP:
                DE_LOG(LogInput, Trace, "Pen Up Event");
                break;
            case SDL_EVENT_PEN_BUTTON_DOWN:
                DE_LOG(LogInput, Trace, "Pen Button Down Event");
                break;
            case SDL_EVENT_PEN_BUTTON_UP:
                DE_LOG(LogInput, Trace, "Pen Button Up Event");
                break;
            case SDL_EVENT_PEN_MOTION:
                DE_LOG(LogInput, Trace, "Pen Motion Event");
                break;
            case SDL_EVENT_PEN_AXIS:
                DE_LOG(LogInput, Trace, "Pen Axis Event");
                break;
            default:
                DE_LOG(LogInput, Error, "Unknown Pen Event");
        }
    }

    void InputSubsystem::ProcessCameraEvent(const SDL_Event& _event)
    {
      switch (_event.type)
{
    case SDL_EVENT_CAMERA_DEVICE_ADDED:
        DE_LOG(LogInput, Trace, "Camera Device Added Event");
        break;
    case SDL_EVENT_CAMERA_DEVICE_REMOVED:
        DE_LOG(LogInput, Trace, "Camera Device Removed Event");
        break;
    case SDL_EVENT_CAMERA_DEVICE_APPROVED:
        DE_LOG(LogInput, Trace, "Camera Device Approved Event");
        break;
    case SDL_EVENT_CAMERA_DEVICE_DENIED:
        DE_LOG(LogInput, Trace, "Camera Device Denied Event");
        break;
    default:
        DE_LOG(LogInput, Error, "Unknown Camera Event");
}
    }

    void InputSubsystem::ProcessRenderEvent(const SDL_Event& _event)
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