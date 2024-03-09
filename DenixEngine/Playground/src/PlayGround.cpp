#include <Denix.h>
#include <Denix/EntryPoint.h>


class Playground: public Denix::Engine
{
public:

	Playground()
	{
		
	}

	~Playground() override
	{
		
	}


	
};

Denix::Engine* Denix::CreateEngine()
{
	return new Playground();
}