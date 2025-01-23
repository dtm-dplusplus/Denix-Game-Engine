#pragma once

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
}
