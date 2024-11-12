
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
	
	if(ImGui::DragInt("Grid Size", &GridSize, 1.0f, 1, 100))
	{
		SpawnGrid();
	}

	ImGui::End();
}

void DevScene::SpawnGrid()
{
	m_SceneObjects.clear();
	
	// Spawn cube grid
	for(int i = 0; i <  GridSize; i++)
	{
		for(int j = 0; j < GridSize; j++)
		{
			SpawnGameObject<Cube>(glm::vec3(i * 2.5f, j * 2.5f, 0.0f));
		}
	}
}