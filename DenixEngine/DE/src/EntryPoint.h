#pragma once

#include "Engine.h"
#include <SDL3/SDL_main.h>

extern Denix::Engine* Denix::CreateEngine();

inline int main(int argc, char* argv[])
{
   auto engine = Denix::CreateEngine();

    engine->Run();
    
    return 0;
}