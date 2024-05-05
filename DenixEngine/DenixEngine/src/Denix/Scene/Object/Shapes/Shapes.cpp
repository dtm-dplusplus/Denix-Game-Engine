#include "Shapes.h"
#include "Denix/Resource/ResourceSubsystem.h"

namespace Denix
{
    Plane::Plane(const ObjectInitializer& _objInit) : GameObject(_objInit)
    {
        m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Plane"));
        m_TransformComponent->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
    }

    Cube::Cube(const ObjectInitializer& _objInit) : GameObject(_objInit)
    {
        m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));
    }
}
