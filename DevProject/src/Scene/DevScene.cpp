
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

	static size_t textureStoreSize = AssetSubsystem::GetTextureStore().size();
	static std::unordered_map<std::string, Ref<Texture>> textureStore = AssetSubsystem::GetTextureStore();
	
	for (int a = 0; a < 100; a++)
	{
		Ref<Actor> actor;
		if (a % 2 == 0)
			actor = SpawnActor<Cube>();
		else
			actor = SpawnActor<Sphere>();
		
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
	}

	// Static Plane
	{
		m_StatActor = SpawnActor<Plane>();
		m_StatActor->GetTransformComponent()->SetMoveability(Moveability::Static);
		m_StatActor->GetTransformComponent()->GetScale() = {4.0f, 0.01f, 4.0f};
	}
}

void DevScene::Update(float _deltaTime)
{
	Scene::Update(_deltaTime);

	if (pxscene)
	{
		pxscene->simulate(_deltaTime);
		pxscene->fetchResults(true);
	}
}

void DevScene::DebugUI(float _deltaTime)
{
	Scene::DebugUI(_deltaTime);

	ImGui::Begin(GetName().c_str());
	if(ImGui::Button("Spawn Cube"))
	{
		m_DyActor = SpawnActor<Cube>();
	}
	if (ImGui::Button("Create Scene"))
	{
		physx::PxSceneDesc sceneDesc = physx::PxSceneDesc(PhysicsSubsystem::m_PxPhysics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -m_Gravity, 0.0f);
		sceneDesc.cpuDispatcher	= PhysicsSubsystem::m_PxDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		pxscene = PhysicsSubsystem::m_PxPhysics->createScene(sceneDesc);

		if(physx::PxPvdSceneClient* pvdClient = pxscene->getScenePvdClient())
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
		pxShape = PhysicsSubsystem::m_PxPhysics->createShape(physx::PxBoxGeometry(0.5f,0.5f,0.5f), *PhysicsSubsystem::m_PxMaterial);
		
		pxActor = PhysicsSubsystem::m_PxPhysics->createRigidDynamic(physx::PxTransform(0.0f, 10.0f, 0.0f));
		pxActor2 = PhysicsSubsystem::m_PxPhysics->createRigidDynamic(physx::PxTransform(0.0f, 14.0f, 0.0f));
		pxActor->attachShape(*pxShape);
		pxActor2->attachShape(*pxShape);
		
		pxscene->addActor(*pxActor);
		pxscene->addActor(*pxActor2);
	}
	if (ImGui::Button("Remove Scene"))
	{
		//PX_RELEASE(pxActor)
		//PX_RELEASE(pxActor2)
		PX_RELEASE(pxscene)
	
	}
	if (ImGui::Button("Remove Cube"))
	{
		if (m_DyActor)
		{
			m_PxScene->removeActor(*m_DyActor->GetPhysicsComponent()->m_PxActor);
		}
	}
	ImGui::End();
}