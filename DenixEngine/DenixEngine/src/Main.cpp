#include "Engine.h"

int main(int argc, char* args[])
{
	const auto engine = new Denix::Engine();

	engine->Run();

	delete engine;

	return 0;
}
