#pragma once

#include "Denix/Scene/GameObject.h"

namespace Denix
{
	class Plane : public GameObject
	{
	public:
		Plane(const ObjectInit& _objInit = { "Plane" });
		void Update(float _deltaTime) override;
	};

	class Cube : public GameObject
	{
	public:
		Cube(const ObjectInit& _objInit = { "Cube" });

		void Update(float _deltaTime) override;

	};

	class Sphere : public GameObject
	{
	public:
		Sphere(const ObjectInit& _objInit = { "Sphere" });

		void Update(float _deltaTime) override;

	};
}
