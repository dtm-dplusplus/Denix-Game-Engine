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
	class PlaygroundScene final : public Scene
	{
	public:
		PlaygroundScene(): Scene({ "PlaygroundScene" }) {}

		bool Load() override;
		void Update(float _deltaTime) override;

		Ref<Cube> TestCube;
		Ref<ModelObject> TestModel;
		Ref<PointLight> PntLight;
		Ref<SpotLight> SptLight;
	};
}
