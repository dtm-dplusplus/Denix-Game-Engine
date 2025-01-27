#include "BallActor.h"

#include "Denix/Asset/AssetSubsystem.h"

BallActor::BallActor()
{
    m_ModelComponent->SetModel(Denix::AssetSubsystem::GetModel("Content\\Engine\\models\\SM_Sphere.obj"));
    m_TransformComponent->GetTransform().Scale = glm::vec3(0.25f);
    m_PhysicsComponent->SetSimulatePhysics(true);
    m_PhysicsComponent->m_Mass = Mass;
}

void BallActor::BeginScene()
{
    Actor::BeginScene();
}
