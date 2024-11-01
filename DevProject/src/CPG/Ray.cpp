#include "Ray.h"

#include "Denix/Resource/ResourceSubsystem.h"

using namespace Denix;

float Ray::m_MoveSpeed = 0.1f;

Ray::Ray(): Cube({"Ray"})
{
    m_TransformComponent->SetPosition(m_Origin);
    m_TransformComponent->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    m_TransformComponent->SetScale(glm::vec3(0.1f, 1.0f, 0.1f));
    m_TransformComponent->SetMoveability(Moveability::Dynamic);
    m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Cylinder"));
    m_RenderComponent->SetMaterial([]
    {
        Ref<Material> mat = MakeRef<Material>(); mat->SetBaseColor(glm::vec3(1.0f)); return mat;
    }());
}

void Ray::OnCollision(Ref<GameObject>& _other, CollisionData& _collision)
{
    Cube::OnCollision(_other, _collision);

    if(const auto& mat = _other->GetRenderComponent()->GetMaterial())
    {
        m_RenderComponent->GetMaterial()->SetBaseColor(mat->GetBaseColor());
        DE_LOG(LogScene, Info, "Ray collided with object: {}", _other->GetName())
    }
}

void Ray::EndPlay()
{
    Cube::EndPlay();
    m_RenderComponent->GetMaterial()->SetBaseColor(glm::vec3(1.0f));
}