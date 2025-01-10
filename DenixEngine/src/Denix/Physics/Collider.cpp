#include "Collider.h"
#include "Denix/Asset/AssetSubsystem.h"

using namespace Denix;

CubeCollider::CubeCollider()
{
	m_ColliderType = (int)ColliderType::Cube;
	m_MeshComponent->SetModel(AssetSubsystem::GetModel("SM_Cube"));

}

SphereCollider::SphereCollider()
{
	m_ColliderType = (int)ColliderType::Sphere;
	m_MeshComponent->SetModel(AssetSubsystem::GetModel("SM_Sphere"));// Will change to sphere
}