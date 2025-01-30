#include "BallActor.h"

#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Audio/AudioSubsystem.h"

using namespace Denix;

BallActor::BallActor()
{
    m_ModelComponent->SetModel(Denix::AssetSubsystem::GetModel("Content\\Engine\\models\\SM_Sphere.obj"));
    m_TransformComponent->GetTransform().Scale = glm::vec3(0.25f);
    m_PhysicsComponent->m_SimulatePhysics = true;
    m_PhysicsComponent->m_ColliderType = ColliderType::Sphere;
    m_PhysicsComponent->m_Mass = Mass;
}

void BallActor::BeginScene()
{
    Actor::BeginScene();
}

void BallActor::OnCollisionEnter(const Denix::Ref<Actor>& _other, const glm::vec3& _normal, const glm::vec3& _point)
{
    if(_other && !m_IsDead)
    {
        if(_other->GetName().find("CubeActor") != std::string::npos)
        {
            // Clear the color
            if(Ref<Material> material = _other->GetRenderComponent()->GetMaterial())
            {
                material->ClearBaseTexture();
                material->SetBaseColor(glm::vec4(0.8f));
            }

            if (Ref<AudioClip> clip = Denix::AssetSubsystem::GetAudioClip("Content\\Audio\\hit.wav"))
            {
                AudioSubsystem::PlayAudioClipSingle(clip);
            }
        }
    }
}

CubeActor::CubeActor() : Actor({"CubeActor"})
{
    m_ModelComponent->SetModel(Denix::AssetSubsystem::GetModel("Content\\Engine\\models\\SM_Cube.obj"));
    //m_TransformComponent->GetTransform().Scale = glm::vec3(0.5f);
    m_PhysicsComponent->m_SimulatePhysics = true;
    m_PhysicsComponent->m_ColliderType = ColliderType::Cube;
}
