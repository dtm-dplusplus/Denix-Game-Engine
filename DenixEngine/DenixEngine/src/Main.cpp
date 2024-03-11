#include "Engine.h"

int main(int argc, char* args[])
{
	Logger::Initialize();

	const auto engine = new Denix::Engine();

	engine->Run();

	delete engine;

	Logger::Deinitialize();
	return 0;
}
