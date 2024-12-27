
#include "DevScene.h"

#include <random>

#include "imgui.h"

using namespace Denix;

void DevScene::BeginScene()
{
	Scene::BeginScene();

	//m_GridSpawner.SpawnGrid(shared_from_this());
}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);
}

void DevScene::DebugUI(float _deltaTime)
{
	Scene::DebugUI(_deltaTime);

	ImGui::Begin(GetName().c_str());
	ImGui::Text("Scene Objetcs Size: %d", GetActorCount());
	ImGui::DragInt("Grid Size", &m_GridSpawner.GridSize, 1.0f, 1, 100);
	if(ImGui::Button("Spawn Grid"))
	{
		m_GridSpawner.SpawnGrid(shared_from_this());
	}
	ImGui::End();
}