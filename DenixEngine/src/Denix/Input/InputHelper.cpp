#include "InputHelper.h"

#include <SDL3/SDL_keyboard.h>

using namespace Denix;

std::string Denix::GetKeyName(KeyCode _key)
{
    return SDL_GetKeyName(static_cast<SDL_Keycode>(_key));
}

KeyCode Denix::GetKeyFromName(const std::string& _key)
{
    return static_cast<KeyCode>(SDL_GetKeyFromName(_key.c_str()));
}
