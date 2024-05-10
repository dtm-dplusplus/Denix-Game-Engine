#pragma once

#include "Denix/Scene/Scene.h"
#include "Denix/Video/GL/Model.h"

namespace Denix
{
	class PhysObject : public GameObject
	{
	public:
		PhysObject(const ObjectInitializer& _objInit = { "PhysObject" });

		void OnCollision(Ref<GameObject> _other, CollisionData& _collison) override
		{
			DE_LOG(Log, Info, "PhysObject OnCollision")
		}


		void OnTriggerEnter(Ref<GameObject> _other) override
		{
			DE_LOG(Log, Info, "PhysObject OnTriggerEnter")
		}

		void OnTriggerExit(Ref<GameObject> _other) override
		{
			DE_LOG(Log, Info, "PhysObject OnTriggerExit")
		}

		void OnTriggerStay(Ref<GameObject> _other) override
		{
		}
	};

	class PhysicsScene final : public Scene
	{
	public:
		PhysicsScene(): Scene({ "Physics Scene" }) {}

		bool Load() override;
		void Update(float _deltaTime) override;

		Ref<PhysObject> PhysicsObject;
		Ref<class Cube> TestCube;
		Ref<PointLight> PntLight;
		Ref<SpotLight> SptLight;
	};
}
