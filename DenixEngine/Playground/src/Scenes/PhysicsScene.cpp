#include "PhysicsScene.h"
#include "imgui.h"
#include "Denix/Core/TimerSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/Object/Shapes/Shapes.h"
#include "Denix/Video/Renderer/RendererSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Core/Math.h"

namespace Denix
{
	PhysObject::PhysObject(const ObjectInitializer& _objInit) : Cube(_objInit)
	{
		m_TransformComponent->SetMoveability(Moveability::Dynamic);
	}

	bool PhysicsScene::Load()
	{
		Scene::Load();

		
		float planeSize = 10.0f;
		for (int i = 0; i < 7; i++)
		{
			Ref <Plane> floor = MakeRef<Plane>();
			floor->GetPhysicsComponent()->CollisionDetectionEnabled() = true;
			floor->GetPhysicsComponent()->IsColliderVisible() = false;
			floor->GetTransformComponent()->SetScale(glm::vec3(planeSize, 0.01f, planeSize * 2));
			floor->GetTransformComponent()->SetPosition(glm::vec3(planeSize * 2.0f * i, i * -5.0f, 0.0f));
			BaseMatParam& base = floor->GetRenderComponent()->GetMaterial()->GetBaseParam();
			base.IsTexture = true;
			base.Texture = ResourceSubsystem::GetTexture("DefaultTexture");
			m_SceneObjects.push_back(floor);
		}

		for (int i = 0; i < 5; i++)
		{
			Ref<Sphere> sphere = MakeRef<Sphere>(ObjectInitializer("Sphere 1"));
			sphere->GetTransformComponent()->SetMoveability(Moveability::Dynamic);
			sphere->GetPhysicsComponent()->IsColliderVisible() = false;
			sphere->GetPhysicsComponent()->SetSimulatePhysics(true);
			sphere->GetPhysicsComponent()->GetLinearDrag() = 0.2f;
			sphere->GetPhysicsComponent()->GetAngularDrag() = 0.5f;
			sphere->GetPhysicsComponent()->GetMass() = 10.0f;
			sphere->GetTransformComponent()->SetPosition({ -2.0f, 3.0f, -3.0f + i * 3.0f });
			BaseMatParam& base = sphere->GetRenderComponent()->GetMaterial()->GetBaseParam();
			base.IsTexture = true;
			base.Texture = ResourceSubsystem::GetTexture("Brick");
			m_SceneObjects.push_back(sphere);
			Spheres.push_back(sphere);
		}

		return true;
	}
	void PhysicsScene::BeginScene()
	{
		Scene::BeginScene();

		m_ActiveCamera->GetTransformComponent()->SetPosition({ -25.0f, 20.0f, 25.0f });
		m_ActiveCamera->GetTransformComponent()->SetRotation({-25.0f,-35.0f,0.0f});
		m_DirLight->GetLightDirection().z = -0.54f;
		m_DirLight->SetAmbientIntensity(0.8f);
	}
	void PhysicsScene::BeginPlay()
	{
		Scene::BeginPlay();

		float i = 1;
		for (auto& obj : Spheres)
		{
			obj->GetPhysicsComponent()->AddImpulse(Sphere1StartImpulse *  i++ * 1.5f);
		}
	}
}