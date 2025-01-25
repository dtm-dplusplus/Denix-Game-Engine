#include "PhysicsComponent.h"

#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Scene/Component/TransformComponent.h"

namespace Denix
{
    PhysicsComponent::PhysicsComponent(): Component(ObjectInit("Physics Component")), m_Collider(nullptr)
    {
    }


    Ref<Collider> PhysicsComponent::GetCollider() const
    { return m_Collider; }

    Ref<Collider>& PhysicsComponent::GetCollider()
    { return m_Collider; }

    void PhysicsComponent::SetCollider(const Ref<Collider>& _collider)
    {
        m_Collider = _collider;
    }

    void PhysicsComponent::BeginScene()
    {
        Component::BeginScene();

        SetupPhysX();

        // Register the physics component with the physics subsystem
        if (m_SimulatePhysics) PhysicsSubsystem::RegisterComponent(shared_from_this());
    }

    void PhysicsComponent::BeginPlay()
    {
        Component::BeginPlay();
    }

    void PhysicsComponent::EndPlay()
    {
        Component::EndPlay();

    }

    void PhysicsComponent::EndScene()
    {
        PhysicsSubsystem::UnregisterComponent(shared_from_this());

        Component::EndScene();
    }

    void PhysicsComponent::Update(float _deltaTime)
    {
        Component::Update(_deltaTime);

        if (m_PxActor && m_SimulatePhysics && SceneSubsystem::GetSceneState() == SceneState::Playing)
        {
            physx::PxTransform tform = m_PxActor->getGlobalPose();
            
            auto parent = m_Parent.lock();
            parent->m_TransformComponent->m_Transform.Position = {tform.p.x, tform.p.y, tform.p.z};
            parent->m_TransformComponent->m_Transform.Rotation = Math::Degrees(glm::eulerAngles(glm::quat(tform.q.w, tform.q.x, tform.q.y, tform.q.z)));
        }
    }

    void PhysicsComponent::AddImpulse(const glm::vec3& _impulse) const
    {
        if (!m_PxActor) return;
        
        if (const auto dynamicActor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            dynamicActor->addForce({ _impulse.x, _impulse.y, _impulse.z }, physx::PxForceMode::eIMPULSE);
            return;
        }
        
        DE_LOG(LogPhysics, Error, "Cannot add impulse to a static actor {}", m_Parent.lock()->GetName());    
    }

    void PhysicsComponent::AddTorque(const glm::vec3& _torque) const
    {
        if (!m_PxActor) return;

        if (const auto dynamicActor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            dynamicActor->addTorque({ _torque.x, _torque.y, _torque.z }, physx::PxForceMode::eFORCE);
            return;
        }

        DE_LOG(LogPhysics, Error, "Cannot add torque to a static actor {}", m_Parent.lock()->GetName());
    }

    void PhysicsComponent::AddForce(const glm::vec3& _force) const
    {
        if (!m_PxActor) return;

        if (const auto dynamicActor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            dynamicActor->addForce({ _force.x, _force.y, _force.z }, physx::PxForceMode::eFORCE);
            return;
        }

        DE_LOG(LogPhysics, Error, "Cannot add force to a static actor {}", m_Parent.lock()->GetName());
    }

    void PhysicsComponent::AddAcceleration(const glm::vec3& _acceleration) const
    {
        if (!m_PxActor) return;

        if (const auto dynamicActor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            dynamicActor->addForce({ _acceleration.x, _acceleration.y, _acceleration.z }, physx::PxForceMode::eACCELERATION);
            return;
        }

        DE_LOG(LogPhysics, Error, "Cannot add acceleration to a static actor {}", m_Parent.lock()->GetName());
    }
    
    void PhysicsComponent::SetupPhysX()
    {
        auto parent = m_Parent.lock();
        const auto transform = parent->m_TransformComponent->m_Transform;
        const auto scaleHalf = transform.Scale * 0.5f;
        
        switch (m_ColliderType)
        {
        case ColliderType::Plane:
        {
            m_PxShape = PhysicsSubsystem::m_PxPhysics->createShape(physx::PxBoxGeometry(transform.Scale.x, 0.01f, transform.Scale.z), *PhysicsSubsystem::m_PxMaterial);
        } break;
            
        case ColliderType::Cube:
            {
                m_PxShape = PhysicsSubsystem::m_PxPhysics->createShape(physx::PxBoxGeometry(scaleHalf.x, scaleHalf.y, scaleHalf.z), *PhysicsSubsystem::m_PxMaterial);
            } break;

        case ColliderType::Sphere:
            {
                m_PxShape = PhysicsSubsystem::m_PxPhysics->createShape(physx::PxSphereGeometry(scaleHalf.x), *PhysicsSubsystem::m_PxMaterial);
            } break;
        }
        
        switch (parent->m_TransformComponent->m_Moveability)
        {
        case 0:
            {
                m_PxActor = PhysicsSubsystem::m_PxPhysics->createRigidStatic(physx::PxTransform(transform.Position.x, transform.Position.y, transform.Position.z));
            } break;

        case 1:
            {
                m_PxActor = PhysicsSubsystem::m_PxPhysics->createRigidDynamic(physx::PxTransform(transform.Position.x, transform.Position.y, transform.Position.z));
                UpdatePxDynamicActor(m_PxActor->is<physx::PxRigidDynamic>());
            } break;
        }
        m_PxActor->userData = parent.get();
        m_PxActor->attachShape(*m_PxShape);
    }

    
    void PhysicsComponent::UpdatePxDynamicActor(physx::PxRigidDynamic* _actor)
    {
        if (!_actor) return;

        _actor->setMass(m_Mass);
        _actor->setLinearDamping(m_LinearDrag);
        _actor->setAngularDamping(m_AngularDrag);
        _actor->setLinearVelocity({m_Velocity.x, m_Velocity.y, m_Velocity.z});
        _actor->setAngularVelocity({m_AngularVelocity.x, m_AngularVelocity.y, m_AngularVelocity.z});

        _actor->setContactSlopCoefficient(m_PxSlopCoefficient);

        _actor->setMassSpaceInertiaTensor({m_MomentOfInertia, m_MomentOfInertia, m_MomentOfInertia});
        physx::PxRigidBodyExt::updateMassAndInertia(static_cast<physx::PxRigidBody&>(*m_PxActor), m_Mass);
        _actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
    }

  

}
