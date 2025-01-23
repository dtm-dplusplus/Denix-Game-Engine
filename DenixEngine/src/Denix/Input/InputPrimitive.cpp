#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "InputPrimitive.h"

#include "Denix/Core/Log/Logger.h"

using namespace Denix;

void Keyboard::ProcessKeyEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        m_KeysDown.insert(static_cast<KeyCode>(event.key.key));
        if (m_KeyboardLogging) DE_LOG(LogInput, Trace, "Key Down Event. Key: {}", SDL_GetKeyName(event.key.key))
    }
    else if (event.type == SDL_EVENT_KEY_UP)
        {
        m_KeysUp.insert(static_cast<KeyCode>(event.key.key));
        if (m_KeyboardLogging)  DE_LOG(LogInput, Trace, "Key Up Event. Key: {}", SDL_GetKeyName(event.key.key))
    }
    else
    {
        DE_LOG(LogInput, Error, "Unknown Keyboard Event")
    }
}

void Mouse::ProcessMouseEvent(const SDL_Event& _event)
{
    /**< Mouse moved */
    if (_event.type == SDL_EVENT_MOUSE_MOTION)
    {
        //m_MouseData.X = _event.motion.x;
        //m_MouseData.Y = _event.motion.y;
        //m_MouseData.RelX = _event.motion.xrel;
        //m_MouseData.RelY = _event.motion.yrel;
        if (m_MouseMotionLogging) DE_LOG(LogInput, Trace, "Mouse Motion Event. X: {} Y: {} RelX: {} RelY: {}", m_MouseData.X, m_MouseData.Y, m_MouseData.RelX, m_MouseData.RelY)
    }
    
    /**< Mouse button pressed */
    else if (_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (_event.button.button == SDL_BUTTON_LEFT) m_MouseData.Left = true;
        if (_event.button.button == SDL_BUTTON_RIGHT) m_MouseData.Right = true;
        if (_event.button.button == SDL_BUTTON_MIDDLE) m_MouseData.Middle = true;
        if (_event.button.button == SDL_BUTTON_X1) m_MouseData.Side1 = true;
        if (_event.button.button == SDL_BUTTON_X2) m_MouseData.Side2 = true;
        if (m_MouseInputLogging) DE_LOG(LogInput, Trace, "Mouse Button Down Event. Button: {}", _event.button.button)
    }

    /**< Mouse button released */
    else if (_event.type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            if (_event.button.button == SDL_BUTTON_LEFT) m_MouseData.Left = false;
            if (_event.button.button == SDL_BUTTON_RIGHT) m_MouseData.Right = false;
            if (_event.button.button == SDL_BUTTON_MIDDLE) m_MouseData.Middle = false;
            if (_event.button.button == SDL_BUTTON_X1) m_MouseData.Side1 = false;
            if (_event.button.button == SDL_BUTTON_X2) m_MouseData.Side2 = false;
        if (m_MouseInputLogging) DE_LOG(LogInput, Trace, "Mouse Button Up Event. Button: {}", _event.button.button)
        }

    /**< Mouse wheel motion */ 
    else if (_event.type == SDL_EVENT_MOUSE_WHEEL)
    {
        m_MouseData.WheelY = _event.wheel.y;
    }
    else
    {
        DE_LOG(LogInput, Error, "Unknown Mouse Event")
    }
}

bool Keyboard::IsKeyDown(KeyCode key) const
{
    return m_SDL_KeyboardState[SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(key), NULL)];
}

bool Keyboard::IsKeyUp(KeyCode key) const
{
    return m_KeysUp.contains(key);    
}
bool Keyboard::IsKeyHold(KeyCode key) const {
    return m_SDL_KeyboardState[SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(key), NULL)];
}