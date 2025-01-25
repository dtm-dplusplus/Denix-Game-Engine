#include "Shapes.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Core/File/FileSubsystem.h"
#include "Denix/Physics/Collider.h"

namespace Denix
{
    Plane::Plane() : Actor({ "Plane" })
    {
    	m_ClassName = "Plane";
        m_MeshComponent->SetModel(AssetSubsystem::GetModel("Content\\Engine\\models\\SM_Plane.obj"));
        m_TransformComponent->GetScale().y = 0.01f;

    	m_PhysicsComponent->m_ColliderType = ColliderType::Plane;
    }

    Cube::Cube() : Actor({ "Cube" })
    {
    	m_ClassName = "Cube";
		m_MeshComponent->SetModel(AssetSubsystem::GetModel("Content\\Engine\\models\\SM_Cube.obj"));

    	m_PhysicsComponent->m_ColliderType = ColliderType::Cube;
    	m_TransformComponent->SetMoveability(Moveability::Dynamic);
    }

	Sphere::Sphere() : Actor({ "Sphere" })
	{
    	m_ClassName = "Sphere";
		m_MeshComponent->SetModel(AssetSubsystem::GetModel("Content\\Engine\\models\\SM_Sphere.obj"));
    	m_PhysicsComponent->m_ColliderType = ColliderType::Sphere;
	}
}