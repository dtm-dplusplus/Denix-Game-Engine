#include "C:/Users/Denis/Documents/Programming Projects/Denix-Game-Engine/Build/DenixEngine/CMakeFiles/DenixEngine.dir/Debug/cmake_pch.hxx"
#include "InputPrimitive.h"

#include "Denix/Core/Log/Logger.h"

using namespace Denix;

void Keyboard::ProcessKeyEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        KeyDown.insert(static_cast<KeyCode>(event.key.key));
        if (m_KeyboardLogging) DE_LOG(LogInput, Trace, "Key Down Event. Key: {}", SDL_GetKeyName(event.key.key))
    }
    else if (event.type == SDL_EVENT_KEY_UP)
        {
        KeyUp.insert(static_cast<KeyCode>(event.key.key));
        if (m_KeyboardLogging)  DE_LOG(LogInput, Trace, "Key Up Event. Key: {}", SDL_GetKeyName(event.key.key))
    }
    else
    {
        DE_LOG(LogInput, Error, "Unknown Keyboard Event")
    }
}

bool Keyboard::IsKeyDown(KeyCode key) const
{
    return keyStates[SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(key), NULL)];
}

bool Keyboard::IsKeyUp(KeyCode key) const
{
    return KeyUp.contains(key);    
}
bool Keyboard::IsKeyHold(KeyCode key) const {
    return keyStates[SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(key), NULL)];
}