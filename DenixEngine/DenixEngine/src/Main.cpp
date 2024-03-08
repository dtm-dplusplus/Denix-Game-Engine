#include "Engine.h"

int main(int argc, char* args[])
{
	const std::unique_ptr<Denix::Engine> engine = std::make_unique<Denix::Engine>();

	engine->Run();

	return 0;
}
