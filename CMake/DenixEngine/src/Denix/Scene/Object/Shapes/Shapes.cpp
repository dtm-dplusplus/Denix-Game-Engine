#include "Shapes.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Physics/Collider.h"
namespace Denix
{
    Plane::Plane(const ObjectInitializer& _objInit) : GameObject(_objInit)
    {
        m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Plane"));
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
        CastRef<CubeCollider>(m_PhysicsComponent->GetCollider())->GetDimensions().y = 0.01f;
        m_TransformComponent->GetScale().y = 0.01f;
    }

    void Plane::Update(float _deltaTime)
    {
        GameObject::Update(_deltaTime);
        
    }

    Cube::Cube(const ObjectInitializer& _objInit) : GameObject(_objInit)
    {
		m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Cube"));
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
    }

    void Cube::Update(float _deltaTime)
    {
        GameObject::Update(_deltaTime);
    }

	Sphere::Sphere(const ObjectInitializer& _objInit) : GameObject(_objInit)
	{
		m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Sphere"));
		m_PhysicsComponent->SetCollider(MakeRef<SphereCollider>());
	}

    void Sphere::Update(float _deltaTime)
    {
        GameObject::Update(_deltaTime);
    }
}