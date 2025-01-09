#include "Shapes.h"
#include "Denix/Resource/ResourceSubsystem.h"
#include "Denix/Physics/Collider.h"
namespace Denix
{
    Plane::Plane(const ObjectInit& _objInit) : Actor(_objInit)
    {
        m_ClassName = "Plane";
        m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Plane"));
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
        CastRef<CubeCollider>(m_PhysicsComponent->GetCollider())->GetDimensions().y = 0.01f;
        m_TransformComponent->GetScale().y = 0.01f;
    }

    void Plane::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        Actor::Update(_deltaTime, _waitCounter);
        
    }

    Cube::Cube(const ObjectInit& _objInit) : Actor(_objInit)
    {
        m_ClassName = "Cube";
		m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Cube"));
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
    }

    void Cube::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        Actor::Update(_deltaTime, _waitCounter);
    }

	Sphere::Sphere(const ObjectInit& _objInit) : Actor(_objInit)
	{
        m_ClassName = "Sphere";
		m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Sphere"));
		m_PhysicsComponent->SetCollider(MakeRef<SphereCollider>());
	}

    void Sphere::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        Actor::Update(_deltaTime, _waitCounter);
    }
}