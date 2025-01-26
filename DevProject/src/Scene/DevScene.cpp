
#include "DevScene.h"

#include "imgui.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Core/Time/TimerSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/Actor/Shapes.h"
#include "GEP/GEPActor.h"

using namespace Denix;

void DevScene::BeginScene()
{
	Scene::BeginScene();

	//SpawnActor<Plane>()->GetTransformComponent()->GetScale() = { 10.0f, 0.01f, 10.0f };
}

void DevScene::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
{
	Scene::Update(_deltaTime, _waitCounter);

}

void DevScene::DebugUI(float _deltaTime, const Ref<Counter>& _waitCounter)
{
	Scene::DebugUI(_deltaTime, _waitCounter);

	ImGui::Begin(GetName().c_str());
	if(ImGui::Button("Spawn Cube"))
	{
		m_DyActor = SpawnActor<GEPActor>({0.0f, 2.5f, 0.0f});
		m_DyActor->GetPhysicsComponent()->SimulatePhysics() = true;
	}
	if (ImGui::Button("Reset Cube"))
	{
		m_DyActor->Destroy();
		m_DyActor = SpawnActor<Cube>();
	}
	ImGui::End();
}