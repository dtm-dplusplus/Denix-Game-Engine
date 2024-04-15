#pragma once

#include "Denix/Scene/Scene.h"
#include "Denix/Video/GL/Model.h"

namespace Denix
{
	class ModelObject : public GameObject
	{
	public:
		ModelObject() : GameObject({"Model"})
		{
			TestModel = MakeRef<Model>();
		}

		Ref<Model> TestModel;
	};

	class PhysObject : public GameObject
	{
		public:
		PhysObject() : GameObject({ "PhysObject" })
		{
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

	class PlaygroundScene final : public Scene
	{
	public:
		PlaygroundScene(): Scene({ "PlaygroundScene" }) {}

		bool Load() override;
		void Update(float _deltaTime) override;

		Ref<PhysObject> TestPhysObject;
		Ref<Cube> TestCube;
		Ref<ModelObject> CubeModel;
		Ref<PointLight> PntLight;
		Ref<SpotLight> SptLight;
	};
}
