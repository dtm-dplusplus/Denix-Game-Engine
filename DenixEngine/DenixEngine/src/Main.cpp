#include "Engine.h"
#include <memory>

int main(int argc, char* args[])
{
	const std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	engine->Run();

	return 0;
}
