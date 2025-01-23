#include "InputSubsystem.h"

#include "backends/imgui_impl_sdl3.h"
#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Video/WindowSubsystem.h"

namespace Denix
{
    void InputSubsystem::Initialize()
    {
        DE_LOG(LogInput, Warn, "Input Subsystem Initializing");
        Subsystem::Initialize();
        
        m_WindowRef = WindowSubsystem::GetWindow();
        DE_ASSERT(m_WindowRef.lock(), "Input: Window reference is invalid");

        Keyboard::m_KeyboardLogging = true;
        Mouse::m_MouseInputLogging = true;
        Mouse::m_MouseMotionLogging = false;
        GetDevices();

        m_Keyboard = MakeRef<Keyboard>();
        DE_ASSERT(m_Keyboard, "Input: Keyboard reference is invalid");

        m_Mouse = MakeRef<Mouse>();
        DE_ASSERT(m_Mouse, "Input: Mouse reference is invalid");
        
        DE_LOG(LogInput, Info, "Input Subsystem Initialized");
    }

    void InputSubsystem::Deinitialize()
    {
        DE_LOG(LogInput, Trace, "Input Subsystem Deinitializing");
        m_Keyboard.reset();
        m_Mouse.reset();
        Subsystem::Deinitialize();
        DE_LOG(LogInput, Trace, "Input Subsystem Deinitialized");
    }

    void InputSubsystem::Update(float _deltaTime)
    {
        DE_PROFILE(Input Update)
        Subsystem::Update(_deltaTime);
        DE_PROFILE_END(Input Update)
    }

    bool InputSubsystem::IsKeyDown(const KeyCode _key)
    {
        return s_Instance->m_Keyboard->IsKeyDown(_key);	

    }

    bool InputSubsystem::IsKeyUp(const KeyCode _key)
    {
        return s_Instance->m_Keyboard->IsKeyUp(_key);
    }

    void InputSubsystem::GetDevices()
    {
        int keyboardCount = 0;
        if (SDL_KeyboardID* keyboardIDs = SDL_GetKeyboards(&keyboardCount))
        {
            for (int i = 0; i < keyboardCount; i++)
            {
                
                DE_LOG(LogInput, Info, "Keyboard ID: {} Name: {}", keyboardIDs[i], SDL_GetKeyboardNameForID(keyboardIDs[i]))
            }
        }
        else
        {
            DE_LOG(LogInput, Warn, "Failed to get Keyboard IDs. SDL_Error: {}", SDL_GetError())
        }

        int mouseCount = 0;
        if (SDL_MouseID* mouseIDs = SDL_GetMice(&mouseCount))
        {
            for (int i = 0; i < mouseCount; i++)
            {
                DE_LOG(LogInput, Info, "Mouse ID: {} Name: {}", mouseIDs[i], SDL_GetMouseNameForID(i))
            }
        }
        else
        {
            DE_LOG(LogInput, Error, "Failed to get Mouse IDs. SDL_Error: {}", SDL_GetError())
        }

        int gamepadCount = 0;
        if (SDL_JoystickID* gamepadIDs = SDL_GetGamepads(&gamepadCount))
        {
            for (int i = 0; i < gamepadCount; i++)
            {
                DE_LOG(LogInput, Info, "Gamepad ID: {} Name: {}", gamepadIDs[0], SDL_GetGamepadNameForID(gamepadIDs[0]))
            }
        }

        int joystickCount = 0;
        if (SDL_JoystickID* joystickIDs = SDL_GetJoysticks(&joystickCount))
        {
            for (int i = 0; i < joystickCount; i++)
            {
                DE_LOG(LogInput, Info, "Joystick ID: {}", joystickIDs[i])
            }
        }

        int touchCount = 0;
        if (SDL_TouchID* touchIDs = SDL_GetTouchDevices(&touchCount))
        {
            for (int i = 0; i < touchCount; i++)
            {
                DE_LOG(LogInput, Info, "Touch ID: {}", touchIDs[i])
            }
        }

        int sensorCount = 0;
        if (SDL_SensorID* sensorIDs = SDL_GetSensors(&sensorCount))
        {
            for (int i = 0; i < sensorCount; i++)
            {
                DE_LOG(LogInput, Info, "Sensor ID: {}", sensorIDs[i])
            }
        }

       int cameraCount = 0;
        if (SDL_CameraID* cameraIDs = SDL_GetCameras(&cameraCount))
        {
            for (int i = 0; i < cameraCount; i++)
            {
                DE_LOG(LogInput, Info, "Camera ID: {}", cameraIDs[i])
            }
        }
    }

    void InputSubsystem::ProcessInputEvent(const SDL_Event& _event)
    {
        /* Keyboard events */
        if (_event.type >= SDL_EVENT_KEY_DOWN && _event.type <= SDL_EVENT_TEXT_EDITING_CANDIDATES)
        {
            m_Keyboard->ProcessKeyEvent(_event);
        }

        /* Mouse events */
        else if (_event.type >= SDL_EVENT_MOUSE_MOTION && _event.type <= SDL_EVENT_MOUSE_REMOVED)
        {
            m_Mouse->ProcessMouseEvent(_event);
        }

        /* Text Events */
        else if (_event.type == SDL_EVENT_TEXT_EDITING || _event.type == SDL_EVENT_TEXT_INPUT)
        {
            DE_LOG(LogInput, Trace, "Text Event")
        }

        /* Gamepad events */
        else if (_event.type >= SDL_EVENT_GAMEPAD_AXIS_MOTION && _event.type <= SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED)
        {
            DE_LOG(LogInput, Trace, "Gamepad Event")

            /* Gamepad events */
            //case SDL_EVENT_GAMEPAD_AXIS_MOTION: break; /**< Gamepad axis motion */
            //case SDL_EVENT_GAMEPAD_BUTTON_DOWN: break; /**< Gamepad button pressed */
            //case SDL_EVENT_GAMEPAD_BUTTON_UP: break; /**< Gamepad button released */
            //case SDL_EVENT_GAMEPAD_ADDED: break; /**< A new gamepad has been inserted into the system */
            //case SDL_EVENT_GAMEPAD_REMOVED: break; /**< A gamepad has been removed */
            //case SDL_EVENT_GAMEPAD_REMAPPED: break; /**< The gamepad mapping was updated */
            //case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN: break; /**< Gamepad touchpad was touched */
            //case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION: break; /**< Gamepad touchpad finger was moved */
            //case SDL_EVENT_GAMEPAD_TOUCHPAD_UP: break; /**< Gamepad touchpad finger was lifted */
            //case SDL_EVENT_GAMEPAD_SENSOR_UPDATE: break; /**< Gamepad sensor was updated */
            //case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE: break; /**< Gamepad update is complete */
            //case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED: break; /**< Gamepad Steam handle has changed */
        }
        /* Joystick events */
        else if (_event.type >= SDL_EVENT_JOYSTICK_AXIS_MOTION && _event.type <= SDL_EVENT_JOYSTICK_UPDATE_COMPLETE)
        {
            //DE_LOG(LogInput, Trace, "Joystick Event")
            /* Joystick events */
            /*case SDL_EVENT_JOYSTICK_AXIS_MOTION: break; /**< Joystick axis motion #1#
            case SDL_EVENT_JOYSTICK_BALL_MOTION: break; /**< Joystick trackball motion #1#
            case SDL_EVENT_JOYSTICK_HAT_MOTION: break; /**< Joystick hat position change #1#
            case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
                {
                   
                }break; /**< Joystick button pressed #1#
            case SDL_EVENT_JOYSTICK_BUTTON_UP:
                {
                   
                }break; /**< Joystick button released #1#
            case SDL_EVENT_JOYSTICK_ADDED: break; /**< A new joystick has been inserted into the system #1#
            case SDL_EVENT_JOYSTICK_REMOVED: break; /**< An opened joystick has been removed #1#
            case SDL_EVENT_JOYSTICK_BATTERY_UPDATED: break; /**< Joystick battery level change #1#
            case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE: break; /**< Joystick update is complete #1#*/
        }

        /* Touch events */
        else if (_event.type >= SDL_EVENT_FINGER_DOWN && _event.type <= SDL_EVENT_FINGER_MOTION)
        {
            DE_LOG(LogInput, Trace, "Touch Event")
            /* Touch events */
            //case SDL_EVENT_FINGER_DOWN: break;
            //case SDL_EVENT_FINGER_UP: break;
            //case SDL_EVENT_FINGER_MOTION: break;
        }
        else
        {
            DE_LOG(LogInput, Error, "Unknown Input Event")
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
}