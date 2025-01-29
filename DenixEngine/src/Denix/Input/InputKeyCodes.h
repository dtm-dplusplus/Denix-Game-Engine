#pragma once

#include <SDL3/SDL_keycode.h>

namespace Denix
{
    enum class KeyCode
    {
        DEK_UNKNOWN = SDLK_UNKNOWN,
        DEK_RETURN = SDLK_RETURN,
        DEK_ESCAPE = SDLK_ESCAPE,
        DEK_BACKSPACE = SDLK_BACKSPACE,
        DEK_TAB = SDLK_TAB,
        DEK_SPACE = SDLK_SPACE,
        DEK_EXCLAIM = SDLK_EXCLAIM,
        DEK_QUOTE = SDLK_APOSTROPHE,
        DEK_HASH = SDLK_HASH,
        DEK_DOLLAR = SDLK_DOLLAR,
        DEK_PERCENT = SDLK_PERCENT,
        DEK_AMPERSAND = SDLK_AMPERSAND,
        DEK_APOSTROPHE = SDLK_APOSTROPHE,
        DEK_LEFTPAREN = SDLK_LEFTPAREN,
        DEK_RIGHTPAREN = SDLK_RIGHTPAREN,
        DEK_ASTERISK = SDLK_ASTERISK,
        DEK_PLUS = SDLK_PLUS,
        DEK_COMMA = SDLK_COMMA,
        DEK_MINUS = SDLK_MINUS,
        DEK_PERIOD = SDLK_PERIOD,
        DEK_SLASH = SDLK_SLASH,

        // Numbers
        DEK_0 = SDLK_0,
        DEK_1 = SDLK_1,
        DEK_2 = SDLK_2,
        DEK_3 = SDLK_3,
        DEK_4 = SDLK_4,
        DEK_5 = SDLK_5,
        DEK_6 = SDLK_6,
        DEK_7 = SDLK_7,
        DEK_8 = SDLK_8,
        DEK_9 = SDLK_9,

        // Letters
        DEK_A = SDLK_A, DEK_B = SDLK_B, DEK_C = SDLK_C, DEK_D = SDLK_D, DEK_E = SDLK_E, DEK_F = SDLK_F, DEK_G = SDLK_G,
        DEK_H = SDLK_H, DEK_I = SDLK_I, DEK_J = SDLK_J, DEK_K = SDLK_K, DEK_L = SDLK_L, DEK_M = SDLK_M, DEK_N = SDLK_N,
        DEK_O = SDLK_O, DEK_P = SDLK_P, DEK_Q = SDLK_Q, DEK_R = SDLK_R, DEK_S = SDLK_S, DEK_T = SDLK_T, DEK_U = SDLK_U,
        DEK_V = SDLK_V, DEK_W = SDLK_W, DEK_X = SDLK_X, DEK_Y = SDLK_Y, DEK_Z = SDLK_Z,

        // Function Keys
        DEK_F1 = SDLK_F1, DEK_F2 = SDLK_F2, DEK_F3 = SDLK_F3, DEK_F4 = SDLK_F4, DEK_F5 = SDLK_F5,
        DEK_F6 = SDLK_F6, DEK_F7 = SDLK_F7, DEK_F8 = SDLK_F8, DEK_F9 = SDLK_F9, DEK_F10 = SDLK_F10,
        DEK_F11 = SDLK_F11, DEK_F12 = SDLK_F12,

        // Control Keys
        DEK_LCTRL = SDLK_LCTRL, DEK_RCTRL = SDLK_RCTRL,
        DEK_LSHIFT = SDLK_LSHIFT, DEK_RSHIFT = SDLK_RSHIFT,
        DEK_LALT = SDLK_LALT, DEK_RALT = SDLK_RALT,
        DEK_LGUI = SDLK_LGUI, DEK_RGUI = SDLK_RGUI,

        // Arrow Keys
        DEK_UP = SDLK_UP, DEK_DOWN = SDLK_DOWN, DEK_LEFT = SDLK_LEFT, DEK_RIGHT = SDLK_RIGHT,

        // Miscellaneous
        DEK_CAPSLOCK = SDLK_CAPSLOCK, DEK_SCROLLLOCK = SDLK_SCROLLLOCK, DEK_NUMLOCK = SDLK_N
    };
}
