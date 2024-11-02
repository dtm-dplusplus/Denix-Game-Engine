
#include "DevScene.h"

#include "imgui.h"

using namespace Denix;


DevScene::DevScene(): Scene("Dev Scene")
{
}

DevScene::DevScene(const Ref<Asset>& _sceneAsset): Scene(_sceneAsset)
{

}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);
}

void DevScene::DebugUI(float _deltaTime)
{
	Scene::DebugUI(_deltaTime);

	if(ImGui::Begin(m_SceneName.c_str()))
	{

		ImGui::End();
	}
}