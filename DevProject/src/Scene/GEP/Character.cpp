#include "Character.h"

#include <PxScene.h>
#include <characterkinematic/PxController.h>

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Input/InputSubsystem.h"
#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/Camera.h"
#include "Denix/Scene/SceneSubsystem.h"

using namespace Denix;

Character::Character(): Actor({"Character"}), m_JumpForce(0.5f), m_MoveSpeed(0.1f)
{
    m_MeshComponent->SetModel(AssetSubsystem::GetModel("Content/Engine/models/SM_Cube.obj"));
    m_RenderComponent->SetMaterial(MakeRef<Material>());
    m_RenderComponent->GetMaterial()->SetBaseColor({0.0f, 1.0f, 0.0f});
    m_TransformComponent->SetMoveability(Moveability::Dynamic);
    m_PhysicsComponent->m_ColliderType = ColliderType::Cube;
}

void Character::BeginPlay()
{
    Actor::BeginPlay();

    m_PhysicsComponent->m_PxActor->is<physx::PxRigidDynamic>()->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

    if (m_Camera)
    {
        //m_Camera->m_ExternalControl = true;
    }
}

void Character::Update(float _deltaTime)
{
    Actor::Update(_deltaTime);

    const glm::vec3& fwd = GetTransformComponent()->GetForward();
    const glm::vec3& right = GetTransformComponent()->GetRight();
    const glm::vec3& up = GetTransformComponent()->GetUp();
    glm::vec3& pos = m_TransformComponent->GetPosition();

    /*m_Camera->GetTransformComponent()->SetPosition(pos);
    m_Camera->GetTransformComponent()->SetRotation(m_TransformComponent->GetRotation());
    m_Camera->GetTransformComponent()->m_Forward = fwd;
    m_Camera->GetTransformComponent()->m_Right = right;
    m_Camera->GetTransformComponent()->m_Up = up;*/

    glm::vec3 movement(0.0f);

    if (InputSubsystem::IsKeyDown(SDL_SCANCODE_SPACE) && IsOnGround())
    {
        //DE_LOG(LogDevProject, Trace, "Space key is pressed");
        //m_PhysicsComponent->AddImpulse({0.0f, m_JumpForce, 0.0f});
    }

    if (InputSubsystem::IsKeyDown(SDL_SCANCODE_W))
    {
        //DE_LOG(LogDevProject, Trace, "Forward key is pressed");
        movement += fwd * m_MoveSpeed * _deltaTime;
       // movement.z = m_MoveSpeed * _deltaTime;
    }
    if (InputSubsystem::IsKeyDown(SDL_SCANCODE_S))
    {
       // movement -= fwd * m_MoveSpeed * _deltaTime;
    }
    if (InputSubsystem::IsKeyDown(SDL_SCANCODE_A))
    {
      //  movement -= right * m_MoveSpeed * _deltaTime;
    }
    if (InputSubsystem::IsKeyDown(SDL_SCANCODE_D))
    {
      //  movement += right * m_MoveSpeed * _deltaTime;
    }
}

bool Character::IsOnGround()
{
    return true; 
}
