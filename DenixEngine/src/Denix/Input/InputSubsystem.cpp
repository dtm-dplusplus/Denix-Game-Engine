#include "InputSubsystem.h"

#include "backends/imgui_impl_sdl3.h"
#include "Denix/Profile/ProfileSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Video/WindowSubsystem.h"

namespace Denix
{
    void InputSubsystem::Initialize()
    {
        Subsystem::Initialize();
        DE_LOG(LogInput, Warn, "Input Subsystem Initializing");
        m_WindowRef = WindowSubsystem::GetWindow();
        DE_ASSERT(m_WindowRef.lock(), "Input: Window reference is invalid");
        
        m_KeyboardLogging = false;
        m_MouseLogging = false;
        GetDevices();

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
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (_event.button.button == SDL_BUTTON_LEFT) m_MouseData.Left = true;
                if (_event.button.button == SDL_BUTTON_RIGHT) m_MouseData.Right = true;
                if (_event.button.button == SDL_BUTTON_MIDDLE) m_MouseData.Middle = true;
                if (_event.button.button == SDL_BUTTON_X1) m_MouseData.Side1 = true;
                if (_event.button.button == SDL_BUTTON_X2) m_MouseData.Side2 = true;
            }break; /**< Mouse button pressed */
        case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                if (_event.button.button == SDL_BUTTON_LEFT) m_MouseData.Left = false;
                if (_event.button.button == SDL_BUTTON_RIGHT) m_MouseData.Right = false;
                if (_event.button.button == SDL_BUTTON_MIDDLE) m_MouseData.Middle = false;
                if (_event.button.button == SDL_BUTTON_X1) m_MouseData.Side1 = false;
                if (_event.button.button == SDL_BUTTON_X2) m_MouseData.Side2 = false;
            }break; /**< Mouse button released */
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
               
            }break; /**< Joystick button pressed */
        case SDL_EVENT_JOYSTICK_BUTTON_UP:
            {
               
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