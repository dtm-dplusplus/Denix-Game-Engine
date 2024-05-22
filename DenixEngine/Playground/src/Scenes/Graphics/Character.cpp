#include "Character.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Input/InputSubsystem.h"

namespace Denix
{
	Character::Character() : GameObject({ "Character" })
	{
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
		m_PhysicsComponent->IsColliderVisible() = true;
		m_PhysicsComponent->SetSimulatePhysics(false);
		m_PhysicsComponent->GetLinearDrag() = 0.75f;

		m_TransformComponent->SetMoveability(Moveability::Dynamic);
		m_TransformComponent->SetScale(glm::vec3(0.3f));
		m_RenderComponent->GetMaterial()->GetBaseParam().Color = { 1.0f, 0.0f, 0.0f };

		FollowCamera = MakeRef<Camera>();
		FollowCamera->GetTransformComponent()->SetPosition(CameraOffset);
		HitPipe = false;
	}

	void Character::GameUpdate(float _deltaTime)
	{
		GameObject::GameUpdate(_deltaTime);
		
		if (InputSubsystem::IsKeyDown(SDL_SCANCODE_SPACE))
		{
			m_PhysicsComponent->AddImpulse({ 0.0f, JumpForce, 0.0f });
		}
	}

	void Character::OnCollision(Ref<GameObject>& _other, CollisionData& _collision)
	{
		if (!_other) return;

		if (_other->GetFriendlyName().find("Pipe") != std::string::npos)
		{
			DE_LOG(LogPlayground, Info, "Player Collided with {}", _other->GetName())
			HitPipe = true;
			m_PhysicsComponent->CollisionDetectionEnabled() = false;
        }
	}
	void Character::BeginPlay()
	{
		GameObject::BeginPlay();

		HitPipe = false;
	}
}