#include "Character.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Scene/Camera.h"

namespace Denix
{
	Character::Character() : GameObject({ "Character" })
	{
		m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Cube"));
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
		m_PhysicsComponent->IsColliderVisible() = true;
		m_PhysicsComponent->SetSimulatePhysics(true);
		m_PhysicsComponent->GetLinearDrag() = 1.0f;
		m_PhysicsComponent->GetElasticity() = 0.0f;

		m_TransformComponent->SetMoveability(Moveability::Dynamic);
		m_TransformComponent->SetScale(glm::vec3(1.0f, 2.0f, 1.0f));
		
		m_RenderComponent->GetMaterial()->GetBaseParam().Color = { 1.0f, 0.0f, 0.0f };

		m_FollowCamera = MakeRef<Camera>();
		m_FollowCamera->GetTransformComponent()->GetRotation().x = -25.0f;
		m_FollowCamera->SetFov(80.0f);

		m_CameraBoomHeight = 8.0f;
		m_CameraBoomLength = 5.0f;
	}

	void Character::Update(float _deltaTime)
	{
		GameObject::Update(_deltaTime);

		// Camera Movement
		if (InputSubsystem* input = InputSubsystem::Get())
		{
			if (input->IsKeyDown(SDL_SCANCODE_SPACE))
			{
				GetPhysicsComponent()->AddImpulse({ 0.0f, JumpForce, 0.0f });
			}
		}

		m_FollowCamera->GetTransformComponent()->SetPosition(
			m_TransformComponent->GetPosition() + glm::vec3(0.0f, m_CameraBoomHeight, m_CameraBoomLength));
	}
	void Character::OnCollision(Ref<GameObject>& _other, CollisionData& _collision)
	{
		if (!_other) return;

		if (_other->GetFriendlyName().find("Pipe") != std::string::npos)
		{
            //std::cout << "Collided with Pipe" << std::endl;
			DE_LOG(LogPlayground, Info, "Collided with Pipe")
        }
	}
	void Character::BeginPlay()
	{
		GameObject::BeginPlay();

		HitPipe = false;
	}
}