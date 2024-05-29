#include "JumpPawn.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Input/InputSubsystem.h"

namespace Denix
{
	JumpPawn::JumpPawn() : GameObject({ "JumpPawn" })
	{
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
		m_PhysicsComponent->SetSimulatePhysics(false);
		m_PhysicsComponent->GetLinearDrag() = 0.75f;

		m_TransformComponent->SetMoveability(Moveability::Dynamic);
		m_TransformComponent->SetScale(glm::vec3(0.3f));
		m_RenderComponent->GetMaterial()->GetBaseParam().Color = { 1.0f, 0.0f, 0.0f };

		FollowCamera = MakeRef<Camera>();
		FollowCamera->GetTransformComponent()->SetPosition(CameraOffset);
		HitPipe = false;
	}

	void JumpPawn::OnCollision(Ref<GameObject>& _other, CollisionData& _collision)
	{
		if (!_other) return;

		if (_other->GetFriendlyName().find("Pipe") != std::string::npos)
		{
			DE_LOG(LogPlayground, Info, "Player Collided with {}", _other->GetName())
			HitPipe = true;
			m_PhysicsComponent->CollisionDetectionEnabled() = false;
        }
	}
	void JumpPawn::BeginPlay()
	{
		GameObject::BeginPlay();

		HitPipe = false;
	}
}