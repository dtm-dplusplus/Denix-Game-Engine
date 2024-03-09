#include "Denix.h"


class Playground: public Denix::Engine
{
public:

	Playground()
	{
		
	}

	~Playground() override
	{
		
	}


	/*Engine* Denix::CreeateEngine()
	{
		return new Playground();
	}*/
};

int main(int argc, char* args[])
{
	//const auto engine = Denix::CreateEngine();
	//
	//engine->Run();
	//
	//delete engine;

	Denix::Engine::Print();

	return 0;
}