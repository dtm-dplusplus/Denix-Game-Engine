//Using SDL and standard IO
#include "DxPch.h"
#include "Engine.h"

int main(int argc, char* args[])
{
	Engine* engine = new Engine();

	engine->Run();

	delete engine;

	return 0;
}