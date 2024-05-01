#pragma once

#include "Denix/Scene/Scene.h"
#include "Denix/Video/GL/Model.h"

namespace Denix
{
	class PhysObject : public GameObject
	{
	public:
		PhysObject() : GameObject({ "PhysObject" })
		{
			m_Moveability = (int)Moveability::Dynamic;
			m_PhysicsComponent->IsSimulated() = true;
		}

		void OnTriggerEnter() override
		{
			DE_LOG(Log, Info, "PhysObject OnTriggerEnter")
		}

		void OnTriggerExit() override
		{
			DE_LOG(Log, Info, "PhysObject OnTriggerExit")
		}

		void OnTriggerStay() override
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
		Ref<Cube> TestCube;
		Ref<PointLight> PntLight;
		Ref<SpotLight> SptLight;
	};
}
