#include "PlaygroundScene.h"
#include "imgui.h"

#include "Denix/System/ShaderSubSystem.h"


namespace Denix
{
	bool PlaygroundScene::Load()
	{
		Scene::Load();
		//TextureObj = MakeRef<TexObject>();
		//TextureObj->GetTransformComponent()->SetScale(glm::vec3(30.0f));
		//TextureObj->GetRenderComponent()->SetShader(ShaderSubsystem::Get()->GetShader("TextureShader"));
		//m_SceneObjects.push_back(TextureObj);

		BrickWall = MakeRef<Plane>();
		BrickWall->GetTransformComponent()->SetScale(glm::vec3(30.0f));
		BrickWall->GetRenderComponent()->SetShader(ShaderSubsystem::Get()->GetShader("TextureShader"));
		std::string brick = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Textures\\brick.png";

		BrickWall->GetRenderComponent()->LoadTexture(brick);
		m_SceneObjects.push_back(BrickWall);

		Ref<Cube> cube = MakeRef<Cube>();
		cube->GetTransformComponent()->SetScale(glm::vec3(30.0f));
		cube->GetRenderComponent()->SetShader(ShaderSubsystem::Get()->GetShader("TextureShader"));
		m_SceneObjects.push_back(cube);

		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::Begin("Playground Scene");
		
		ImGui::End();
	}
}