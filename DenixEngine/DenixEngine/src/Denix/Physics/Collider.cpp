#include "Collider.h"
#include "Denix/Resource/ResourceSubsystem.h"

using namespace Denix;

const glm::vec4 Collider::m_NoCollisionColor{ glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) };
const glm::vec4 Collider::m_CollisionColor{ glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };

CubeCollider::CubeCollider()
{
	m_ColliderType = (int)ColliderType::Cube;
	m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));

}

CubeCollider::CubeCollider(Ref<RenderComponent> _renderComponent)
{
	m_ColliderType = (int)ColliderType::Cube;
	m_RenderComponent = _renderComponent;
	m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));

}

SphereCollider::SphereCollider()
{
	m_ColliderType = (int)ColliderType::Sphere;
	m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Cube"));// Will change to sphere
}