#include "Shapes.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Physics/Collider.h"
namespace Denix
{
    Plane::Plane(const ObjectInitializer& _objInit) : GameObject(_objInit)
    {
        m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Plane"));
        m_TransformComponent->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
		m_PhysicsComponent->SetCollider(MakeRef<PlaneCollider>());
    }

    Cube::Cube(const ObjectInitializer& _objInit) : GameObject(_objInit)
    {
        m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
    }

	Sphere::Sphere(const ObjectInitializer& _objInit) : GameObject(_objInit)
	{
		m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube")); // Will change to sphere
		m_PhysicsComponent->SetCollider(MakeRef<SphereCollider>());
	}
}
