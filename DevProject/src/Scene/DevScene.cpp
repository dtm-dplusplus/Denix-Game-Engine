
#include "DevScene.h"

#include <random>

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

	// Create a random device to seed the random number generator
	static std::random_device rd;

	// Use the Mersenne Twister engine
	static std::mt19937 gen(rd());

	// Define the range for the random numbers
	std::uniform_int_distribution<> dis(1, 100);

	// Generate and print a random number
	int random_number = dis(gen);
	std::cout << "Random Number: " << random_number << std::endl;

}

void DevScene::DebugUI(float _deltaTime)
{
	Scene::DebugUI(_deltaTime);

	if(ImGui::Begin(m_SceneName.c_str()))
	{

		ImGui::End();
	}
	
}