#include "PlaygroundScene.h"
#include "imgui.h"

#include "Denix/System/ShaderSubSystem.h"


namespace Denix
{
	bool PlaygroundScene::Load()
	{
		Scene::Load();

		Brick = MakeRef<Cube>();
		Brick->GetTransformComponent()->SetScale(glm::vec3(30.0f));
		Brick->GetRenderComponent()->SetShader(ShaderSubsystem::Get()->GetShader("TextureShader"));
		std::string brick = std::filesystem::current_path().parent_path().string() + "\\Playground\\Content\\Textures\\brick.png";

		Brick->GetRenderComponent()->LoadTexture(brick);
		m_SceneObjects.push_back(Brick);

		LightSource = MakeRef<Light>();
		LightSource->GetRenderComponent()->SetShader(ShaderSubsystem::Get()->GetShader("TextureShader"));
		m_SceneObjects.push_back(LightSource);
		return true;
	}

	void PlaygroundScene::Update(float _deltaTime)
	{
		Scene::Update(_deltaTime);

		ImGui::Begin("Playground Scene");
		ImGui::ColorEdit3("Light Color", &LightSource->m_LightColor[0]);
		ImGui::SliderFloat("Ambient Intensity", &LightSource->m_AmbientIntensity, 0.0f, 1.0f);
		ImGui::End();

		LightSource->UseLight();
	}
}