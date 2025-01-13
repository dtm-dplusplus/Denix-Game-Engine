#include "PhysicsComponent.h"

#include "Denix/Physics/PhysicsSubsystem.h"
#include "Denix/Scene/SceneSubsystem.h"
#include "Denix/Scene/Component/TransformComponent.h"

namespace Denix
{
    PhysicsComponent::PhysicsComponent(): Component(ObjectInit("Physics Component")), m_Collider(nullptr)
    {
    }

    PhysicsComponent::~PhysicsComponent()
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
        PhysicsSubsystem::RegisterComponent(shared_from_this());
    }

    void PhysicsComponent::BeginPlay()
    {
        // Register the physics component with the physics subsystem
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

        if (m_PxActor && SceneSubsystem::GetSceneState() == SceneState::Playing)
        {
            physx::PxVec3 pos = m_PxActor->getGlobalPose().p;
            physx::PxQuat rot = m_PxActor->getGlobalPose().q;
            m_Parent->m_TransformComponent->m_Position = {pos.x, pos.y, pos.z};
            m_Parent->m_TransformComponent->m_Rotation = Math::Degrees(glm::eulerAngles(glm::quat(rot.w, rot.x, rot.y, rot.z)));
        }
    }

    void PhysicsComponent::AddImpulse(const glm::vec3& _impulse)
    {
        if (!m_PxActor) return;
        
        if (const auto dynamicActor = m_PxActor->is<physx::PxRigidDynamic>())
        {
            dynamicActor->addForce({ _impulse.x, _impulse.y, _impulse.z }, physx::PxForceMode::eIMPULSE);
            return;
        }
        
        DE_LOG(LogPhysics, Error, "Cannot add impulse to a static actor {}", m_Parent->GetName());    
    }

    void PhysicsComponent::SetupPhysX()
    {
        const auto scale = m_Parent->m_TransformComponent->m_Scale;
        const auto scaleH = m_Parent->m_TransformComponent->m_Scale / 2.0f;
        const auto pos = m_Parent->m_TransformComponent->m_Position;
        const auto rot = Math::Radians(m_Parent->m_TransformComponent->m_Rotation);
        physx::PxTransform tform = physx::PxTransform(pos.x, pos.y, pos.z);
        
        switch (m_ColliderType)
        {
        case ColliderType::Plane:
        {
            m_PxShape = PhysicsSubsystem::m_PxPhysics->createShape(physx::PxBoxGeometry(scale.x, 0.01f, scale.z), *PhysicsSubsystem::m_PxMaterial);
        } break;
            
        case ColliderType::Cube:
            {
                m_PxShape = PhysicsSubsystem::m_PxPhysics->createShape(physx::PxBoxGeometry(scaleH.x, scaleH.y, scaleH.z), *PhysicsSubsystem::m_PxMaterial);
            } break;

        case ColliderType::Sphere:
            {
                m_PxShape = PhysicsSubsystem::m_PxPhysics->createShape(physx::PxSphereGeometry(scaleH.x), *PhysicsSubsystem::m_PxMaterial);
            } break;
        }
        
        switch (m_Parent->m_TransformComponent->m_Moveability)
        {
        case 0:
            {
                m_PxActor = PhysicsSubsystem::m_PxPhysics->createRigidStatic(physx::PxTransform(pos.x, pos.y, pos.z));
            } break;

        case 1:
            {
                m_PxActor = PhysicsSubsystem::m_PxPhysics->createRigidDynamic(physx::PxTransform(pos.x, pos.y, pos.z));
                UpdatePxDynamicActor(m_PxActor->is<physx::PxRigidDynamic>());
            } break;
        }
        m_PxActor->userData = m_Parent.get();
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
