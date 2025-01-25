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
        RegisterComponent();
    }

    void PhysicsComponent::RegisterComponent()
    {
        Component::RegisterComponent();

        PhysicsSubsystem::RegisterComponent(shared_from_this());
    }

    void PhysicsComponent::UnregisterComponent()
    {
        Component::UnregisterComponent();

        PhysicsSubsystem::UnregisterComponent(shared_from_this());
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

    void PhysicsComponent::Update(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        Component::Update(_deltaTime, _waitCounter);

        if (m_PxActor && m_SimulatePhysics && SceneSubsystem::GetSceneState() == SceneState::Playing)
        {
            physx::PxTransform tform = m_PxActor->getGlobalPose();
            
            auto parent = m_Parent.lock();
            parent->m_TransformComponent->m_Transform.Position = {tform.p.x, tform.p.y, tform.p.z};
            parent->m_TransformComponent->m_Transform.Rotation = Math::Degrees(glm::eulerAngles(glm::quat(tform.q.w, tform.q.x, tform.q.y, tform.q.z)));
        }
    }

    void PhysicsComponent::PostUpdate(float _deltaTime, const Ref<Counter>& _waitCounter)
    {
        Component::PostUpdate(_deltaTime, _waitCounter);

        if (!m_PxActor) return;
                
        // Update physx gravity - This only disables gravity, collisions are still detected
        if (m_AttributeFlags & PHYSICS_SIMULATE)
        {
            if (SimulatePhysics())
            {
                m_PxActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
                DE_LOG(LogPhysics, Trace, "Simulating Physics for {}", GetParent()->GetName())
            }
            else
            {
                m_PxActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
                DE_LOG(LogPhysics, Trace, "Stopping Physics Simulation for {}", GetParent()->GetName())
            }
            m_AttributeFlags &= ~PHYSICS_SIMULATE;
        }

        // Update physx collision detection
        if (m_AttributeFlags & PHYSICS_COLLISION)
        {
            if (CollisionDetectionEnabled())
            {
                if (m_PxShape)
                {
                    physx::PxShape* shape; // Adjust if needed
                    m_PxActor->getShapes(&shape, 1);
                   m_PxActor->attachShape(*m_PxShape);
                }
              
                    
                DE_LOG(LogPhysics, Trace, "Collision Detection Enabled for {}", GetParent()->GetName())
            }
            else
            {
                physx::PxShape* shape; // Adjust if needed
                m_PxActor->getShapes(&shape, 1);
                m_PxActor->detachShape(*shape);
                DE_LOG(LogPhysics, Trace, "Collision Detection Disabled for {}", GetParent()->GetName())
            }

            m_AttributeFlags &= ~PHYSICS_COLLISION;
        }

    // Update physx mass
    if (m_AttributeFlags & PHYSICS_MASS)
    {
        SetInertia();
        DE_LOG(LogPhysics, Trace, "Mass set to {} for {}", GetMass(), GetParent()->GetName())
        m_AttributeFlags &= ~PHYSICS_MASS;
    }
    
    if (SimulatePhysics())
    {
        if (physx::PxRigidDynamic* pxActor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            const glm::vec3& pos = GetParent()->GetTransformComponent()->GetPosition();
            pxActor->setGlobalPose(physx::PxTransform(pos.x, pos.y, pos.z));
        }
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

        // Set the actor's shape & user data
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

        // Needs Fixing
        SetInertia();
        _actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
    }

    void PhysicsComponent::SetInertia()
    {
        if (!m_PxShape || m_PxActor) return;
        
        
        // Get geometry type
        physx::PxType type = m_PxShape->getConcreteType();
        physx::PxVec3 inertiaTensor;

        if (type == physx::PxGeometryType::eBOX) {
            physx::PxBoxGeometry box;
            //m_PxShape->getBoxGeometry(box);

            // Compute inertia tensor for a solid box
            float width = box.halfExtents.x * 2.0f;
            float height = box.halfExtents.y * 2.0f;
            float depth = box.halfExtents.z * 2.0f;

            inertiaTensor = physx::PxVec3(
                (1.0f / 12.0f) * m_Mass * (height * height + depth * depth),
                (1.0f / 12.0f) * m_Mass * (width * width + depth * depth),
                (1.0f / 12.0f) * m_Mass * (width * width + height * height)
            );
        }
        else if (type == physx::PxGeometryType::eSPHERE) {
            physx::PxSphereGeometry sphere;
          //  m_PxShape->getSphereGeometry(sphere);

            // Compute inertia tensor for a solid sphere
            float radius = sphere.radius;
            float I = (2.0f / 5.0f) * m_Mass * (radius * radius);
            inertiaTensor = physx::PxVec3(I, I, I);
        }
        else {
            // Default to a uniform tensor for unknown shapes
            inertiaTensor = physx::PxVec3(1.0f, 1.0f, 1.0f) * m_Mass;
        }

        // Apply m_Mass and inertia tensor
        if (physx::PxRigidDynamic* actor =  m_PxActor->is<physx::PxRigidDynamic>())
        {
            actor->setMass(m_Mass);
            actor->setMassSpaceInertiaTensor(inertiaTensor);
        }
        //physx::PxRigidBodyExt::updateMassAndInertia(static_cast<physx::PxRigidBody&>(*m_PxActor), m_Mass);
    }


}
