#pragma once

#include "Denix/Scene/Scene.h"

#include <memory>

namespace Denix
{
	class PlaygroundScene final : public Scene
	{
	public:
		PlaygroundScene(const ObjectInitializer& _objInit = ObjectInitializer::Get());
		~PlaygroundScene() override;

		bool Load() override;
		void Unload() override;

		void BeginScene() override;
		void EndScene() override;
		void Update(float _deltaTime) override;

	private:

	};
}