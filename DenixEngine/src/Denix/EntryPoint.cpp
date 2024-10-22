#pragma once
#include "Engine.h"
#include "Core/Logger.h"

extern Denix::URef<Denix::Engine> Denix::CreateEngine();


int main(int argc, char** argv)
{

    auto engine = Denix::CreateEngine();
    engine->Run();
    
    return 0;
}