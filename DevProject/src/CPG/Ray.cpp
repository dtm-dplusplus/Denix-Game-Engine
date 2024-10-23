#include "Ray.h"

#include "Denix/Resource/ResourceSubsystem.h"

using namespace Denix;

float Ray::m_MoveSpeed = 0.1f;

Ray::Ray(): Cube({"Ray"})
{
 InitRay();
}

Ray::Ray(const glm::vec3& _origin, const glm::vec3& _direction): Cube({"Ray"})
{
    InitRay(_origin, _direction);
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

void Ray::InitRay(const glm::vec3& _origin, const glm::vec3& _direction)
{
    m_Origin = _origin;
    m_Direction = _direction;
    m_TransformComponent->SetPosition(m_Origin);
    m_TransformComponent->SetRotation(m_Direction);
    m_TransformComponent->SetMoveability(Moveability::Dynamic);
    m_RenderComponent->SetMaterial([]
    {
        Ref<Material> mat = MakeRef<Material>(); mat->SetBaseColor(glm::vec3(1.0f)); return mat;
    }());
}
