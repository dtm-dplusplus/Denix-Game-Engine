#pragma once

#include "Denix/Scene/Scene.h"

#include <memory>

namespace Denix
{
	class PlaygroundScene final : public Scene
	{
	public:
		PlaygroundScene(const ObjectInitializer& _objInit = ObjectInitializer("PlaygroundScene")) {}
		~PlaygroundScene() override {}
		bool Load() override;
		void Update(float _deltaTime) override;

		Ref<EqualTriangle> EqualTri;
		Ref<RightTriangle> RightTri;
		Ref<Plane> Square;
	};
}