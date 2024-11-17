
#include "DevScene.h"

#include <random>

#include "imgui.h"

using namespace Denix;

void DevScene::BeginScene()
{
	Scene::BeginScene();

	SpawnGrid();
}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);
}

void DevScene::DebugUI(float _deltaTime)
{
	Scene::DebugUI(_deltaTime);

	ImGui::Begin(GetName().c_str());
	ImGui::Text("Scene Capacity: %d", m_SceneObjects.capacity());
	ImGui::Text("Scene Objetcs Size: %d", m_SceneObjects.size());
	ImGui::DragInt("Grid Size", &GridSize, 1.0f, 1, 100);
	ImGui::SameLine();
	if (ImGui::Button("Update Grid")) SpawnGrid();
	ImGui::End();
}

void DevScene::SpawnGrid()
{
	DE_LOG(LogScene, Info, "Spawning Grid of size: {}", GridSize);
	DE_LOG(LogScene, Info, "Scene Capacity: {}", m_SceneObjects.capacity());
	m_SceneObjects.clear();
	m_SceneObjects.shrink_to_fit();
	DE_LOG(LogScene, Info, "Scene Capacity: {}", m_SceneObjects.capacity());
	
	// Spawn cube grid
	for(int i = 0; i <  GridSize; i++)
	{
		for(int j = 0; j < GridSize; j++)
		{
			SpawnGameObject<Cube>(glm::vec3(i * 2.5f, j * 2.5f, 0.0f));
		}
	}
}