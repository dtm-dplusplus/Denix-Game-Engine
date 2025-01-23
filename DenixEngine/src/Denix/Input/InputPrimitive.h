#pragma once

#include <SDL3/SDL_events.h>
#include <unordered_set>

#include "InputKeyCodes.h"

namespace Denix
{
    struct MouseData
    {
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
        Keyboard(): m_SDL_KeyboardState(SDL_GetKeyboardState(nullptr))
        {
        }
        ~Keyboard()
        {
            m_SDL_KeyboardState = nullptr;
        }
    
        bool IsKeyDown(KeyCode key) const;
        bool IsKeyUp(KeyCode key) const;
        bool IsKeyHold(KeyCode key) const;

        inline static bool m_KeyboardLogging;
        
    private:
        void ProcessKeyEvent(const SDL_Event& event);

        std::unordered_set<KeyCode> m_KeysDown;
        std::unordered_set<KeyCode> m_KeysHold;
        std::unordered_set<KeyCode> m_KeysUp;
        const bool* m_SDL_KeyboardState;

        friend class InputSubsystem;
        friend class EventSubsystem;
    };

    class Mouse
    {
    public:
        Mouse() =default;
        ~Mouse() = default;

        MouseData m_MouseData;

        inline static bool m_MouseInputLogging;
        inline static bool m_MouseMotionLogging;
    private:
        void ProcessMouseEvent(const SDL_Event& _event);

        //std::unordered_set<KeyCode> m_KeysDown;
       //std::unordered_set<KeyCode> m_KeysHold;
       //std::unordered_set<KeyCode> m_KeysUp;
        //const bool* m_SDL_KeyStates;
        uint32_t SDL_State;
        uint32_t SDL_RelativeState;

       
        friend class InputSubsystem;
        friend class EventSubsystem;
        
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
