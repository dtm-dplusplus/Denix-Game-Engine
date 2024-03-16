#include "PlaygroundScene.h"

namespace Denix
{
	PlaygroundScene::PlaygroundScene(const ObjectInitializer& _objInit):
		Scene(_objInit)
	{ 
	}

	PlaygroundScene::~PlaygroundScene()
	{
	}

	bool PlaygroundScene::Load()
	{
		Scene::Load();

		m_SceneObjects.push_back(MakeRef<Cube>());

		return true;
	}

	void PlaygroundScene::Unload()
	{
		Scene::Unload();
	}

	void PlaygroundScene::BeginScene()
	{
		Scene::BeginScene();

	}

	void PlaygroundScene::EndScene()
	{
		Scene::EndScene();
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);
	}
}