#pragma once

#include <SDL3/SDL_events.h>
#include <unordered_set>

#include "InputKeyCodes.h"

namespace Denix
{
    struct MouseData
    {
        uint32_t SDL_State;
        uint32_t SDL_RelativeState;

        // Movement
        float X = 0;
        float Y = 0;
        float RelX = 0;
        float RelY = 0;
        float WheelY = 0;

        // Buttons
        bool Left = false;
        bool Right = false;
        bool Middle = false;
        bool Side1 = false;
        bool Side2 = false;
    };


    class Keyboard
    {
    public:
        void ProcessKeyEvent(const SDL_Event& event);
    
        bool IsKeyDown(KeyCode key) const;
        bool IsKeyUp(KeyCode key) const;
        bool IsKeyHold(KeyCode key) const;

        inline static bool m_KeyboardLogging;
        
    //private:
        std::unordered_set<KeyCode> KeyDown;
        std::unordered_set<KeyCode> KeyHold;
        std::unordered_set<KeyCode> KeyUp;
        const bool* keyStates = SDL_GetKeyboardState(nullptr);
        bool lastKeyStates[SDL_SCANCODE_COUNT] = {false};
    };
    
    class Device
    {
    public:
        Device() = default;
        virtual ~Device() = default;

        std::string m_DeviceName;
        int m_DeviceID;
    };
    
}
