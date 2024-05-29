#include "GameObject.h"

#include "Denix/Resource/ResourceSubsystem.h"


namespace Denix
{
    GameObject::GameObject(const ObjectInitializer& _object_init) : Object(_object_init)
    {
        m_TransformComponent = MakeRef<TransformComponent>(GetName());
        m_Components["Transform"] = m_TransformComponent;

        m_MeshComponent = MakeRef<MeshComponent>(GetName());
        m_Components["Mesh"] = m_MeshComponent;

        m_RenderComponent = MakeRef<RenderComponent>(GetName());
        m_Components["Render"] = m_RenderComponent;

        m_PhysicsComponent = MakeRef<PhysicsComponent>(GetName());
        m_PhysicsComponent->m_ParentTransform = m_TransformComponent;
        m_Components["Physics"] = m_PhysicsComponent;
    }
}