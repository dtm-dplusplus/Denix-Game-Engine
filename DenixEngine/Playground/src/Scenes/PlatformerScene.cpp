#include "PlatformerScene.h"

#include "imgui.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Resource/ResourceSubsystem.h"

Character::Character() : GameObject({"Character"})
{
	m_FollowCamera = MakeRef<Camera>(ObjectInitializer("Follow Camera"));
	
	m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));

	//GetTransformComponent()->SetScale({ 0.25f, 1.0f, 0.25f });

	//m_RenderComponent->SetBaseColorAsTexture(true);
}

void Character::BeginPlay()
{}

void Character::EndPlay()
{}

void Character::BeginScene()
{
	
}

void Character::EndScene()
{}

void Character::Update(float _deltaTime)
{
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
			GetPhysicsComponent()->AddForce({ 0.0f, m_JumpForce, 0.0f });
		}

		// Character Movement
		m_FollowCamera->GetTransformComponent()->SetPosition(m_TransformComponent->GetPosition() + glm::vec3(0.0f, 0.0f, m_CameraBoomLength));
	}
}

bool PlatformerScene::Load()
{
	Scene::Load();

	Player = MakeRef<Character>();
	m_SceneObjects.push_back(Player);
	m_SceneObjects.push_back(Player->m_FollowCamera);

	Ref<Plane> plane = MakeRef<Plane>();
	plane->GetTransformComponent()->SetScale({ 100.0f, 100.0f , 1.0f });
	m_SceneObjects.push_back(plane);

	m_DirLight->SetAmbientIntensity(0.8f);

	return true;
}

void PlatformerScene::Update(float _deltaTime)
{
	ImGui::SetNextWindowPos(ImVec2(1000, 50));
	ImGui::Begin("Playground Tools");
	ImGui::DragFloat("Camera Boom Length", &Player->m_CameraBoomLength, 0.1f, 0.0f, 100.0f);
	ImGui::End();
}

void PlatformerScene::BeginPlay()
{
	Scene::BeginPlay();

}
