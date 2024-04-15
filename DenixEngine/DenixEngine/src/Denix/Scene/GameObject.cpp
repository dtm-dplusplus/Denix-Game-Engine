#include "GameObject.h"

#include "Denix/Resource/ResourceSubsystem.h"

namespace Denix
{
    void GameObject::LateUpdate(float _deltaTime)
    {
        Object::LateUpdate(_deltaTime);

        if (m_MoveabilityChanged)
        {
            // Propogate update to physics systems
            m_PhysicsComponent->SetMoveability(static_cast<Moveability>(m_Moveability));
            m_MoveabilityChanged = false;
        }
    }

    Plane::Plane(const ObjectInitializer& _objInit) : GameObject(_objInit)
    {
        m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_Plane"));
    }

    Cube::Cube(const ObjectInitializer& _objInit): GameObject(_objInit)
    {
        m_MeshComponent->SetMesh(ResourceSubsystem::GetMesh("SM_CUBE"));
    }

    GameObject::GameObject(const ObjectInitializer& _object_init) : Object(_object_init)
    {
        m_TransformComponent = MakeRef<TransformComponent>(GetName());
        m_Components["Transform"] = m_TransformComponent;

        m_MeshComponent = MakeRef<MeshComponent>(GetName());
        m_Components["Mesh"] = m_MeshComponent;

        m_RenderComponent = MakeRef<RenderComponent>(GetName());
        m_Components["Render"] = m_RenderComponent;

        m_PhysicsComponent = MakeRef<PhysicsComponent>(GetName());
        m_PhysicsComponent->m_ActorTransform = m_TransformComponent;
        m_Components["Physics"] = m_PhysicsComponent;
    }
}

