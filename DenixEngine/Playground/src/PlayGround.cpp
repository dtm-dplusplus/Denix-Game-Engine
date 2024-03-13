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

};

Denix::Engine* Denix::CreateEngine()
{
	return new Playground();
}