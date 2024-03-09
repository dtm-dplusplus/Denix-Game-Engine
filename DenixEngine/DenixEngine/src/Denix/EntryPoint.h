#pragma once
#include "Engine.h"

extern Denix::Engine* Denix::CreateEngine();


inline int main(int argc, char** argv)
{
	auto engine = Denix::CreateEngine();
	engine->Run();
	delete engine;
}