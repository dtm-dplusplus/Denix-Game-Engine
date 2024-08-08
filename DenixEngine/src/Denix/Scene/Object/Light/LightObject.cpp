#include "LightObject.h"
#include "Denix/Resource/ResourceSubsystem.h"

Denix::Light::Light(const ObjectInitializer& _objInit):
    GameObject(_objInit),
    m_LightColor(1.0f, 1.0f, 1.0f),
    m_AmbientIntensity(0.8f),
    m_DiffuseIntensity(1.0f)
{
    m_TransformComponent->SetScale(glm::vec3(0.4f));

    m_RenderComponent->SetAffectsLighting(false);
    m_RenderComponent->GetMaterial()->GetBaseParam().Color = glm::vec3(1.0f, 1.0f, 0.0f);
    m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Cube"));

    m_PhysicsComponent->SetCollisionDetectionEnabled(false);
}
