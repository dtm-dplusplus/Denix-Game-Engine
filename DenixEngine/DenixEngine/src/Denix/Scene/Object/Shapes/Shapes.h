#pragma once

#include "Denix/Scene/GameObject.h"

namespace Denix
{
	class Plane : public GameObject
	{
	public:
		Plane(const ObjectInitializer& _objInit = { "Plane" });
	};

	class Cube : public GameObject
	{
	public:
		Cube(const ObjectInitializer& _objInit = { "TestCube" });
	};
}
