#pragma once
#include "Engine.h"
#include "Core/Logger.h"

extern Denix::Engine* Denix::CreateEngine();


int main(int argc, char** argv)
{
	Logger::Initialize();

	auto engine = Denix::CreateEngine();
	engine->Run();
	delete engine;

	Logger::Deinitialize();

	return 0;
}