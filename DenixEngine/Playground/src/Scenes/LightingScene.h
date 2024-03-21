#pragma once
#include "Denix/Scene/Scene.h"

using namespace Denix;

class LightPlane : public GameObject
{
public:
	LightPlane() : GameObject({ "LightPlane" })
	{
	
	}
};

class LightCube : public GameObject
{
public:
	LightCube() : GameObject({ "LightCube" })
	{
		
	}

	Ref<VertexBuffer> NormalBuffer;
};



class LightingScene final : public Denix::Scene
{
public:
	LightingScene(const Denix::ObjectInitializer& _objInit = Denix::ObjectInitializer::Get());
	~LightingScene() override;

	bool Load() override;
	void Unload() override;

	void BeginScene() override;
	void EndScene() override;
	void Update(float _deltaTime) override;
};

