#include "Collider.h"
#include "Denix/Resource/ResourceSubsystem.h"

using namespace Denix;

CubeCollider::CubeCollider()
{
	m_ColliderType = (int)ColliderType::Cube;
	m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Cube"));

}

SphereCollider::SphereCollider()
{
	m_ColliderType = (int)ColliderType::Sphere;
	m_MeshComponent->SetModel(ResourceSubsystem::GetModel("SM_Sphere"));// Will change to sphere
}