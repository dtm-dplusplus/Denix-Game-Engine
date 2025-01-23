#pragma once

#include <string>

#include "InputKeyCodes.h"

namespace Denix
{
    std::string GetKeyName(KeyCode _key);
    KeyCode GetKeyFromName(const std::string& _key);
}
