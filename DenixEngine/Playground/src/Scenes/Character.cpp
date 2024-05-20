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
		m_FollowCamera = MakeRef<Camera>();

		m_TransformComponent->SetMoveability(Moveability::Dynamic);

		m_PhysicsComponent->SetSimulatePhysics(true);
	}

	void Character::Update(float _deltaTime)
	{
		GameObject::Update(_deltaTime);

		// Camera Movement
		if (InputSubsystem* input = InputSubsystem::Get())
		{
			const float moveSpeed = input->IsKeyDown(SDL_SCANCODE_LSHIFT) ? m_SprintSpeed : m_MoveSpeed;

			// XZ 
			if (input->IsKeyDown(SDL_SCANCODE_W))
			{
				GetTransformComponent()->GetPosition() += moveSpeed * m_FollowCamera->GetCameraFront() * _deltaTime;
			}
			if (input->IsKeyDown(SDL_SCANCODE_S))
			{
				GetTransformComponent()->GetPosition() -= moveSpeed * m_FollowCamera->GetCameraFront() * _deltaTime;
			}
			if (input->IsKeyDown(SDL_SCANCODE_A))
			{
				GetTransformComponent()->GetPosition() -= moveSpeed * glm::normalize(glm::cross(m_FollowCamera->GetCameraFront(), m_FollowCamera->GetCameraUp())) * _deltaTime;
			}
			if (input->IsKeyDown(SDL_SCANCODE_D))
			{
				GetTransformComponent()->GetPosition() += moveSpeed * glm::normalize(glm::cross(m_FollowCamera->GetCameraFront(), m_FollowCamera->GetCameraUp())) * _deltaTime;
			}

			if (input->IsKeyDown(SDL_SCANCODE_SPACE))
			{
				GetPhysicsComponent()->AddImpulse({ 0.0f, m_JumpForce, 0.0f });
			}

			// Character Movement
		}

		m_FollowCamera->GetTransformComponent()->SetPosition(m_TransformComponent->GetPosition() + glm::vec3(0.0f, 0.0f, m_CameraBoomLength));
	}
}