#pragma once

#include "Denix/Scene/Actor.h"

namespace Denix
{
	class Plane : public Actor
	{
	public:
		Plane(const ObjectInit& _objInit = { "Plane" });
		void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;
	};

	class Cube : public Actor
	{
	public:
		Cube(const ObjectInit& _objInit = { "Cube" });

		void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

	};

	class Sphere : public Actor
	{
	public:
		Sphere(const ObjectInit& _objInit = { "Sphere" });

		void Update(float _deltaTime, const Ref<Counter>& _waitCounter) override;

	};
}
