#pragma once
#include "Engine.h"
#include "Project.h"
#include "Core/Logger.h"

// extern Denix::Engine* Denix::CreateEngine();


int main(int argc, char** argv)
{
    Logger::Initialize();

    auto engine = new Project;
    engine->Run();
    delete engine;

    Logger::Deinitialize();

    return 0;
}