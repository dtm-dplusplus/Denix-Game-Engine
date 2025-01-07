
#include "DevScene.h"

#include "imgui.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"

using namespace Denix;

void DevScene::BeginScene()
{
	Scene::BeginScene();

	ClearScene();

	/*static size_t textureStoreSize = AssetSubsystem::GetTextureStore().size();
	static std::unordered_map<std::string, Ref<Texture>> textureStore = AssetSubsystem::GetTextureStore();
	
	for (int a = 0; a < 2; a++)
	{
		Ref<Actor> actor;
		actor = SpawnActor<Cube>();
		/*if (a % 2 == 0)
			actor = SpawnActor<Cube>();
		else
			actor = SpawnActor<Sphere>();#1#
		
		actor->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
		actor->GetTransformComponent()->SetPosition({0.0f, 11.0f + a * 2.0f, 0.0f});
		actor->GetPhysicsComponent()->GetAngularVelocity() = {Math::RandF(-5,5),Math::RandF(-5,5),Math::RandF(-5,5)};

		size_t index = Math::Rand(0, textureStoreSize - 1);
		size_t i = 0;

		for (auto texture: textureStore | std::views::keys)
		{
			if (i++ == index)
			{
				Ref<Material> mat = MakeRef<Material>();
				mat->GetBaseTexture() = textureStore[texture];
				actor->GetRenderComponent()->SetMaterial(mat);
				
				break;
			}
		}
	}
	// Static Cube
	{
		m_StatActor = SpawnActor<Cube>();
		m_StatActor->GetTransformComponent()->SetMoveability(Moveability::Static);
		m_StatActor->GetTransformComponent()->GetScale() = {10.0f, 1.0f, 10.0f};
	}*/

	// Static Plane
	/*{
		m_StatActor = SpawnActor<Plane>();
		m_StatActor->GetTransformComponent()->SetMoveability(Moveability::Static);
		m_StatActor->GetTransformComponent()->GetScale() = {4.0f, 0.01f, 4.0f};
	}*/
}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);

}

void DevScene::DebugUI(float _deltaTime)
{
	Scene::DebugUI(_deltaTime);

	ImGui::Begin(GetName().c_str());
	if(ImGui::Button("Spawn Cube"))
	{
		m_DyActor = SpawnActor<Cube>();
	}
	ImGui::End();
}