#include "Shapes.h"
#include "Denix/Asset/AssetSubsystem.h"
#include "Denix/Physics/Collider.h"

namespace Denix
{
    Plane::Plane() : Actor({ "Plane" })
    {
    	m_ClassName = "Plane";
        static std::string planeModelPath = FileSubsystem::GetEngineContentRoot() + "models\\SM_Plane.obj";
        m_MeshComponent->SetModel(AssetSubsystem::GetModel(planeModelPath));
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
        CastRef<CubeCollider>(m_PhysicsComponent->GetCollider())->GetDimensions().y = 0.01f;
        m_TransformComponent->GetScale().y = 0.01f;
    }

    Cube::Cube() : Actor({ "Cube" })
    {
    	m_ClassName = "Cube";
        static std::string cubeModelPath = FileSubsystem::GetEngineContentRoot() + "models\\SM_Cube.obj";
		m_MeshComponent->SetModel(AssetSubsystem::GetModel(cubeModelPath));
		m_PhysicsComponent->SetCollider(MakeRef<CubeCollider>());
    }

	Sphere::Sphere() : Actor({ "Sphere" })
	{
    	m_ClassName = "Sphere";
        static std::string sphereModelPath = FileSubsystem::GetEngineContentRoot() + "models\\SM_Sphere.obj";
		m_MeshComponent->SetModel(AssetSubsystem::GetModel(sphereModelPath));
		m_PhysicsComponent->SetCollider(MakeRef<SphereCollider>());
	}
}