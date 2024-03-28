#pragma once

#include "Denix/Scene/Scene.h"

namespace Denix
{
	class TexPlane : public GameObject
	{
	public:
		TexPlane();

		unsigned int Texture;

		float TexCoords[8] = {
			0.0f, 0.0f,   // bottom left  
			0.0f, 1.0f,  // bottom right
			0.0f, 1.0f,   //  top left
			1.0f, 1.0f // top right
		};
	};

	class PlaygroundScene final : public Scene
	{
	public:
		PlaygroundScene(const ObjectInitializer& _objInit = ObjectInitializer("PlaygroundScene")) {}

		bool Load() override;
		void Update(float _deltaTime) override;

		//Ref<EqualTriangle> EqualTri;
		//Ref<RightTriangle> RightTri;
		//Ref<Plane> Square;
		Ref<TexPlane> TestPlane;
	};
}
