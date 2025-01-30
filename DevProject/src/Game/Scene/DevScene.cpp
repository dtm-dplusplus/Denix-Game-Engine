
#include "DevScene.h"

#include "imgui.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Core/Time/TimerSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/Actor/Shapes.h"

using namespace Denix;

void DevScene::BeginScene()
{
	Scene::BeginScene();

	// Client Logic

}

void DevScene::EndScene()
{
	// Client Logic


    Scene::EndScene();
}

void DevScene::BeginPlay()
{
    Scene::BeginPlay();

	// Client Logic
}

void DevScene::EndPlay()
{
    // Client Logic

    Scene::EndPlay();
}

void DevScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
	Scene::Update(_deltaTime, _waitCounter);

	// Client Logic

}

void DevScene::ToolUpdate(float _deltaTime, const Ref<Counter>& _waitCounter)
{
	Scene::ToolUpdate(_deltaTime, _waitCounter);

    // Client Logic

}