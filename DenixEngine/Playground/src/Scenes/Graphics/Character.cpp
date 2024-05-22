#include "Character.h"
#include "Denix/Scene/Camera.h"

namespace Denix
{
	Character::Character() : GameObject({ "Character" })
	{
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
		m_PhysicsComponent->IsColliderVisible() = true;
		m_PhysicsComponent->SetSimulatePhysics(false);
		m_PhysicsComponent->GetLinearDrag() = 1.0f;
		m_PhysicsComponent->GetElasticity() = 0.0f;

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
		
	}

	void Character::OnCollision(Ref<GameObject>& _other, CollisionData& _collision)
	{
		if (!_other) return;

		if (_other->GetFriendlyName().find("Pipe") != std::string::npos)
		{
			DE_LOG(LogPlayground, Info, "Player Collided with {}", _other->GetName())
			HitPipe = true;
        }
	}
	void Character::BeginPlay()
	{
		GameObject::BeginPlay();

		HitPipe = false;
	}
}