#include "Engine.h"

int main(int argc, char* args[])
{
	Engine* engine = new Engine();

	engine->Run();

	delete engine;

	return 0;
}